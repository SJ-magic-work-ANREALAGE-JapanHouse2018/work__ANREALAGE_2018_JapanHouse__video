/************************************************************
************************************************************/
#include "Effect.h"

/************************************************************
************************************************************/
/********************
modify with:MASK_TYPE
********************/
char EFFECT::FileName_mask[NUM_MASKS][BUF_SIZE] = {
	"mask_Diag.png",
	"mask_Geom0.png",
	"mask_LR.png",
	"mask_LRLR.png",
	"mask_Pyramid.png",
	"mask_UpDown.png",
};

int EFFECT::W_EffectGroup[NUM_EFFECTGROUP] = {
	1, // EFFECTGROUP__NONE,
	
	1, // EFFECTGROUP__DIAG,
	1, // EFFECTGROUP__GEOM0,
	1, // EFFECTGROUP__LR,
	1, // EFFECTGROUP__LRLR,
	1, // EFFECTGROUP__PYRAMID,
	1, // EFFECTGROUP__UPDOWN,
};

/************************************************************
************************************************************/

/******************************
******************************/
EFFECT::EFFECT()
: b_Rev_EffectedArea_inShader(false)
, EffectGroup(EFFECTGROUP__NONE)
, EffectGroup_Detail(0)
{
	/********************
	********************/
	shader_Gray.load( "Gray.vert", "Gray.frag");
	shader_Gray_Inv.load( "Gray_Inv.vert", "Gray_Inv.frag");
	shader_mix.load( "mix.vert", "mix.frag");
	
	/********************
	********************/
	fbo_org.allocate(W_CONTENTS, H_CONTENTS, GL_RGBA);
	fbo_img0.allocate(W_CONTENTS, H_CONTENTS, GL_RGBA);
	fbo_img1.allocate(W_CONTENTS, H_CONTENTS, GL_RGBA);
	
	/********************
	********************/
	for(int i = 0; i < NUM_MASKS; i++){
		if(!checkif_FileExist(FileName_mask[i])) { ERROR_MSG(); std::exit(1); }
		
		img_mask[i].load(FileName_mask[i]);
	}
}

/******************************
******************************/
EFFECT::~EFFECT()
{
}

/******************************
******************************/
void EFFECT::exit()
{
}

/******************************
******************************/
bool EFFECT::checkif_FileExist(const char* FileName)
{
	if(ofFile::doesFileExist(FileName)){
		// printf("loaded file of %s\n", FileName);
		return true;
		
	}else{
		printf("> %s not exist\n", FileName);
		return false;
	}
}

/******************************
******************************/
void EFFECT::setup()
{
}

/******************************
******************************/
void EFFECT::update()
{
}

/******************************
******************************/
void EFFECT::print_EffectType()
{
#ifndef SJ_RELEASE

	printf("> EffectType:\n");
	
	switch(EffectGroup){
		case EFFECTGROUP__NONE:
			printf("None\n");
			break;
			
		case EFFECTGROUP__DIAG:
			printf("Diag/ ");
			switch(EffectGroup_Detail){
				case MASK_DIAG__COLOR__COLOR_DIAG:
					printf("Color/ Color Diag\n");
					break;
				case MASK_DIAG__COLOR__GRAY_DIAG:
					printf("Color/ Gray Diag\n");
					break;
				case MASK_DIAG__COLOR__GRAYINV_DIAG:
					printf("Color/ GrayInv Diag\n");
					break;
				case MASK_DIAG__COLOR__GRAYINV:
					printf("Color/ GrayInv\n");
					break;
			}
			break;
			
		case EFFECTGROUP__GEOM0:
			printf("Geom0/ ");
			switch(EffectGroup_Detail){
				case MASK_GEOM0__COLOR__COLOR_LR:
					printf("Color/ Color LR\n");
					break;
				case MASK_GEOM0__COLOR__GRAY_LR:
					printf("Color/ Gray LR\n");
					break;
				case MASK_GEOM0__COLOR__GRAYINV_LR:
					printf("Color/ GrayInv LR\n");
					break;
				case MASK_GEOM0__COLOR__GRAYINV:
					printf("Color/ GrayInv\n");
					break;
			}
			break;
			
		case EFFECTGROUP__LR:
			printf("LR/ ");
			switch(EffectGroup_Detail){
				case MASK_LR__COLOR__COLOR_LR:
					printf("Color/ Color LR\n");
					break;
				case MASK_LR__COLOR__GRAY_LR:
					printf("Color/ Gray LR\n");
					break;
				case MASK_LR__COLOR__GRAYINV_LR:
					printf("Color/ GrayInv\n");
					break;
			}
			break;
			
		case EFFECTGROUP__LRLR:
			printf("LRLR/ ");
			switch(EffectGroup_Detail){
				case MASK_LRLR__COLOR__COLOR_LR:
					printf("Color/ Color LR\n");
					break;
				case MASK_LRLR__COLOR__GRAY_LR:
					printf("Color/ Gray LR\n");
					break;
				case MASK_LRLR__COLOR__GRAYINV_LR:
					printf("Color/ GrayInv\n");
					break;
			}
			break;
			
		case EFFECTGROUP__PYRAMID:
			printf("Pyramid/ ");
			switch(EffectGroup_Detail){
				case MASK_PYRAMID__COLOR__COLOR_LR:
					printf("Color/ Color LR\n");
					break;
				case MASK_PYRAMID__COLOR__GRAY_LR:
					printf("Color/ Gray LR\n");
					break;
				case MASK_PYRAMID__COLOR__GRAYINV_LR:
					printf("Color/ GrayInv LR\n");
					break;
				case MASK_PYRAMID__COLOR__GRAYINV:
					printf("Color/ GrayInv\n");
					break;
			}
			break;
			
		case EFFECTGROUP__UPDOWN:
			printf("UpDown/ ");
			switch(EffectGroup_Detail){
				case MASK_UPDOWN__COLOR__COLOR_UPDOWN:
					printf("Color/ Color UpDown\n");
					break;
				case MASK_UPDOWN__COLOR__GRAY_UPDOWN:
					printf("Color/ Gray UpDown\n");
					break;
				case MASK_UPDOWN__COLOR__GRAYINV_UPDOWN:
					printf("Color/ GrayInv UpDown\n");
					break;
			}
			break;
			
		default:
			break;
	}
	
#endif
}

/******************************
******************************/
void EFFECT::Dice_Effect()
{
	Dice_EffectGroup();
	Dice_EffectGroup_Detail();
	
	print_EffectType();
}

/******************************
******************************/
void EFFECT::Dice_EffectGroup()
{
	EFFECTGROUP LastEffectGroup = EffectGroup;
	const int MAX_RETRY = 5;
	int counter = 0;

	while( (EffectGroup == LastEffectGroup) && (counter < MAX_RETRY) ){
		EffectGroup = EFFECTGROUP(Dice_index(W_EffectGroup, NUM_EFFECTGROUP));
		counter++;
	}
}

/******************************
******************************/
void EFFECT::Dice_EffectGroup_Detail()
{
	/********************
	********************/
	switch(EffectGroup){
		case EFFECTGROUP__NONE:
			EffectGroup_Detail = 0;
			break;
			
		case EFFECTGROUP__DIAG:
			EffectGroup_Detail = Dice_index(NUM_EFFECTGROUP_DETAIL__DIAG);
			break;
			
		case EFFECTGROUP__GEOM0:
			EffectGroup_Detail = Dice_index(NUM_EFFECTGROUP_DETAIL__GEOM0);
			break;
			
		case EFFECTGROUP__LR:
			EffectGroup_Detail = Dice_index(NUM_EFFECTGROUP_DETAIL__LR);
			break;
			
		case EFFECTGROUP__LRLR:
			EffectGroup_Detail = Dice_index(NUM_EFFECTGROUP_DETAIL__LRLR);
			break;
			
		case EFFECTGROUP__PYRAMID:
			EffectGroup_Detail = Dice_index(NUM_EFFECTGROUP_DETAIL__PYRAMID);
			break;
			
		case EFFECTGROUP__UPDOWN:
			EffectGroup_Detail = Dice_index(NUM_EFFECTGROUP_DETAIL__UPDOWN);
			break;
			
		default:
			EffectGroup_Detail = 0;
			break;
	}
	
	/********************
	********************/
	b_Rev_EffectedArea_inShader = false;
	// b_Rev_EffectedArea_inShader = bool(Dice_index(2));
}

/******************************
******************************/
int EFFECT::Dice_index(int NUM)
{
	return (int)( ((double)rand() / ((double)RAND_MAX + 1)) * (NUM) );
}

/******************************
******************************/
int EFFECT::Dice_index(int *Weight, int NUM)
{
	/***********************
	cal sum of Weight
	***********************/
	int TotalWeight = 0;
	int i;
	for(i = 0; i < NUM; i++){
		TotalWeight += Weight[i];
	}
	
	if(TotalWeight == 0) { ERROR_MSG(); std::exit(1); }
	
	/***********************
	random number
	***********************/
	// int RandomNumber = rand() % TotalWeight;
	int RandomNumber = (int)( ((double)rand() / ((double)RAND_MAX + 1)) * (TotalWeight) );
	
	/***********************
	define play mode
	***********************/
	int index;
	for(TotalWeight = 0, index = 0; /* */; index++){
		TotalWeight += Weight[index];
		if(RandomNumber < TotalWeight) break;
	}
	
	return index;
}

/******************************
******************************/
void EFFECT::draw(ofxHapPlayer& video, ofFbo& fbo_target)
{
	/********************
	********************/
	DrawTool__Video_to_fbo(video, fbo_org);
	
	/********************
	********************/
	switch(EffectGroup){
		case EFFECTGROUP__NONE:
			draw__Normal(fbo_target);
			break;
			
		case EFFECTGROUP__DIAG:
			switch(EffectGroup_Detail){
				case MASK_DIAG__COLOR__COLOR_DIAG:
					draw__maskDiag__Color__ColorDiag(fbo_target);
					break;
				case MASK_DIAG__COLOR__GRAY_DIAG:
					draw__maskDiag__Color__GrayDiag(fbo_target);
					break;
				case MASK_DIAG__COLOR__GRAYINV_DIAG:
					draw__maskDiag__Color__GrayInvDiag(fbo_target);
					break;
				case MASK_DIAG__COLOR__GRAYINV:
					draw__maskDiag__Color__GrayInv(fbo_target);
					break;
			}
			break;
			
		case EFFECTGROUP__GEOM0:
			switch(EffectGroup_Detail){
				case MASK_GEOM0__COLOR__COLOR_LR:
					draw__maskGeom0__Color__ColorLR(fbo_target);
					break;
				case MASK_GEOM0__COLOR__GRAY_LR:
					draw__maskGeom0__Color__GrayLR(fbo_target);
					break;
				case MASK_GEOM0__COLOR__GRAYINV_LR:
					draw__maskGeom0__Color__GrayInvLR(fbo_target);
					break;
				case MASK_GEOM0__COLOR__GRAYINV:
					draw__maskGeom0__Color__GrayInv(fbo_target);
					break;
			}
			break;
			
		case EFFECTGROUP__LR:
			switch(EffectGroup_Detail){
				case MASK_LR__COLOR__COLOR_LR:
					draw__maskLR__Color__ColorLR(fbo_target);
					break;
				case MASK_LR__COLOR__GRAY_LR:
					draw__maskLR__Color__GrayLR(fbo_target);
					break;
				case MASK_LR__COLOR__GRAYINV_LR:
					draw__maskLR__Color__GrayInvLR(fbo_target);
					break;
			}
			break;
			
		case EFFECTGROUP__LRLR:
			switch(EffectGroup_Detail){
				case MASK_LRLR__COLOR__COLOR_LR:
					draw__maskLRLR__Color__ColorLR(fbo_target);
					break;
				case MASK_LRLR__COLOR__GRAY_LR:
					draw__maskLRLR__Color__GrayLR(fbo_target);
					break;
				case MASK_LRLR__COLOR__GRAYINV_LR:
					draw__maskLRLR__Color__GrayInvLR(fbo_target);
					break;
			}
			break;
			
		case EFFECTGROUP__PYRAMID:
			switch(EffectGroup_Detail){
				case MASK_PYRAMID__COLOR__COLOR_LR:
					draw__maskPyramid__Color__ColorLR(fbo_target);
					break;
				case MASK_PYRAMID__COLOR__GRAY_LR:
					draw__maskPyramid__Color__GrayLR(fbo_target);
					break;
				case MASK_PYRAMID__COLOR__GRAYINV_LR:
					draw__maskPyramid__Color__GrayInvLR(fbo_target);
					break;
				case MASK_PYRAMID__COLOR__GRAYINV:
					draw__maskPyramid__Color__GrayInv(fbo_target);
					break;
			}
			break;
			
		case EFFECTGROUP__UPDOWN:
			switch(EffectGroup_Detail){
				case MASK_UPDOWN__COLOR__COLOR_UPDOWN:
					draw__maskUpDown__Color__ColorUpDown(fbo_target);
					break;
				case MASK_UPDOWN__COLOR__GRAY_UPDOWN:
					draw__maskUpDown__Color__GrayUpDown(fbo_target);
					break;
				case MASK_UPDOWN__COLOR__GRAYINV_UPDOWN:
					draw__maskUpDown__Color__GrayInvUpDown(fbo_target);
					break;
			}
			break;
			
		default:
			break;
	}
}

/******************************
******************************/
void EFFECT::DrawTool__Video_to_fbo(ofxHapPlayer& video, ofFbo& fbo)
{
	fbo.begin();
	
	ofClear(0, 0, 0, 0);
	ofSetColor(255, 255, 255, 255);
	video.draw(0, 0, fbo.getWidth(), fbo.getHeight());
	
	fbo.end();
}

/******************************
******************************/
void EFFECT::DrawTool__fbo_mix(ofFbo& fbo_target, MASK_TYPE MaskType)
{
	fbo_target.begin();
	shader_mix.begin();
	
	ofClear(0, 0, 0, 0);
	ofSetColor(255, 255, 255, 255);
	
	shader_mix.setUniform1i( "b_Rev", int(b_Rev_EffectedArea_inShader) );
	shader_mix.setUniformTexture( "texture1", fbo_img1.getTextureReference(), 1 ); 
	shader_mix.setUniformTexture( "texture_mask", img_mask[MaskType].getTexture(), 2 ); 
	
	fbo_img0.draw(0, 0);
	
	shader_mix.end();
	fbo_target.end();
}

/******************************
******************************/
void EFFECT::DrawTool__FboToFbo_ColorAndMirror(ofFbo& fbo_from, ofFbo& fbo_to, EFFECT_COLOR EffectColor, EFFECT_MIRRROR EffectMirror)
{
	/********************
	********************/
	if(EffectColor == EFFECT_COLOR__RGB)			{ /* nothing. */ }
	else if(EffectColor == EFFECT_COLOR__GRAY)		{ shader_Gray.begin(); }
	else if(EffectColor == EFFECT_COLOR__GRAY_INV)	{ shader_Gray_Inv.begin(); }
	
	/********************
	********************/
	fbo_to.begin();
	ofPushMatrix();
		/********************
		********************/
		if(EffectMirror == EFFECT_MIRROR__LR){
			ofTranslate(fbo_from.getWidth(), 0);
			ofScale(-1, 1, 1);
		}else if(EffectMirror == EFFECT_MIRROR__UP_DOWN){
			ofTranslate(0, fbo_from.getHeight());
			ofScale(1, -1, 1);
		}else if(EffectMirror == EFFECT_MIRROR__DIAG){
			ofTranslate(fbo_from.getWidth(), fbo_from.getHeight());
			ofScale(-1, -1, 1);
		}else{
			// none.
		}
		
		/********************
		********************/
		ofClear(0, 0, 0, 0);
		ofSetColor(255, 255, 255, 255);
		fbo_from.draw(0, 0);
		
	ofPopMatrix();
	fbo_to.end();
	
	/********************
	********************/
	if(EffectColor == EFFECT_COLOR__RGB)			{ /* nothing. */ }
	else if(EffectColor == EFFECT_COLOR__GRAY)		{ shader_Gray.end(); }
	else if(EffectColor == EFFECT_COLOR__GRAY_INV)	{ shader_Gray_Inv.end(); }
}

/******************************
******************************/
void EFFECT::draw__Normal(ofFbo& fbo_target)
{
	fbo_target.begin();
	
	ofClear(0, 0, 0, 0);
	ofSetColor(255, 255, 255, 255);
	
	fbo_org.draw(0, 0);
	
	fbo_target.end();
}

/******************************
******************************/
void EFFECT::draw__maskLR__Color__ColorLR(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__RGB, EFFECT_MIRROR__LR);
	
	DrawTool__fbo_mix(fbo_target, MASK_LR);
}

/******************************
******************************/
void EFFECT::draw__maskLR__Color__GrayLR(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__GRAY, EFFECT_MIRROR__LR);
	
	DrawTool__fbo_mix(fbo_target, MASK_LR);
}

/******************************
******************************/
void EFFECT::draw__maskLR__Color__GrayInvLR(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__GRAY_INV, EFFECT_MIRROR__LR);
	
	DrawTool__fbo_mix(fbo_target, MASK_LR);
}

/******************************
******************************/
void EFFECT::draw__maskLRLR__Color__ColorLR(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__RGB, EFFECT_MIRROR__LR);
	
	DrawTool__fbo_mix(fbo_target, MASK_LRLR);
}

/******************************
******************************/
void EFFECT::draw__maskLRLR__Color__GrayLR(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__GRAY, EFFECT_MIRROR__LR);
	
	DrawTool__fbo_mix(fbo_target, MASK_LRLR);
}

/******************************
******************************/
void EFFECT::draw__maskLRLR__Color__GrayInvLR(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__GRAY_INV, EFFECT_MIRROR__LR);
	
	DrawTool__fbo_mix(fbo_target, MASK_LRLR);
}

/******************************
******************************/
void EFFECT::draw__maskUpDown__Color__ColorUpDown(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__RGB, EFFECT_MIRROR__UP_DOWN);
	
	DrawTool__fbo_mix(fbo_target, MASK_UPDOWN);
}

/******************************
******************************/
void EFFECT::draw__maskUpDown__Color__GrayUpDown(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__GRAY, EFFECT_MIRROR__UP_DOWN);
	
	DrawTool__fbo_mix(fbo_target, MASK_UPDOWN);
}

/******************************
******************************/
void EFFECT::draw__maskUpDown__Color__GrayInvUpDown(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__GRAY_INV, EFFECT_MIRROR__UP_DOWN);
	
	DrawTool__fbo_mix(fbo_target, MASK_UPDOWN);
}

/******************************
******************************/
void EFFECT::draw__maskDiag__Color__ColorDiag(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__RGB, EFFECT_MIRROR__DIAG);
	
	DrawTool__fbo_mix(fbo_target, MASK_DIAG);
}

/******************************
******************************/
void EFFECT::draw__maskDiag__Color__GrayDiag(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__GRAY, EFFECT_MIRROR__DIAG);
	
	DrawTool__fbo_mix(fbo_target, MASK_DIAG);
}

/******************************
******************************/
void EFFECT::draw__maskDiag__Color__GrayInvDiag(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__GRAY_INV, EFFECT_MIRROR__DIAG);
	
	DrawTool__fbo_mix(fbo_target, MASK_DIAG);
}

/******************************
******************************/
void EFFECT::draw__maskDiag__Color__GrayInv(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__GRAY_INV, EFFECT_MIRROR__NONE);
	
	DrawTool__fbo_mix(fbo_target, MASK_DIAG);
}

/******************************
******************************/
void EFFECT::draw__maskPyramid__Color__ColorLR(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__RGB, EFFECT_MIRROR__LR);
	
	DrawTool__fbo_mix(fbo_target, MASK_PYRAMID);
}

/******************************
******************************/
void EFFECT::draw__maskPyramid__Color__GrayLR(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__GRAY, EFFECT_MIRROR__LR);
	
	DrawTool__fbo_mix(fbo_target, MASK_PYRAMID);
}

/******************************
******************************/
void EFFECT::draw__maskPyramid__Color__GrayInvLR(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__GRAY_INV, EFFECT_MIRROR__LR);
	
	DrawTool__fbo_mix(fbo_target, MASK_PYRAMID);
}

/******************************
******************************/
void EFFECT::draw__maskPyramid__Color__GrayInv(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__GRAY_INV, EFFECT_MIRROR__NONE);
	
	DrawTool__fbo_mix(fbo_target, MASK_PYRAMID);
}

/******************************
******************************/
void EFFECT::draw__maskGeom0__Color__ColorLR(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__RGB, EFFECT_MIRROR__LR);
	
	DrawTool__fbo_mix(fbo_target, MASK_GEOM0);
}

/******************************
******************************/
void EFFECT::draw__maskGeom0__Color__GrayLR(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__GRAY, EFFECT_MIRROR__LR);
	
	DrawTool__fbo_mix(fbo_target, MASK_GEOM0);
}

/******************************
******************************/
void EFFECT::draw__maskGeom0__Color__GrayInvLR(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__GRAY_INV, EFFECT_MIRROR__LR);
	
	DrawTool__fbo_mix(fbo_target, MASK_GEOM0);
}

/******************************
******************************/
void EFFECT::draw__maskGeom0__Color__GrayInv(ofFbo& fbo_target)
{
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img0, EFFECT_COLOR__RGB, EFFECT_MIRROR__NONE);
	DrawTool__FboToFbo_ColorAndMirror(fbo_org, fbo_img1, EFFECT_COLOR__GRAY_INV, EFFECT_MIRROR__NONE);
	
	DrawTool__fbo_mix(fbo_target, MASK_GEOM0);
}



