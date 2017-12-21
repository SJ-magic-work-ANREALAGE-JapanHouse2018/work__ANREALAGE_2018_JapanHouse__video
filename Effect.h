/************************************************************
How to add Effect.
	add mask.png
	
	add
		enum MASK_TYPE
		FileName_mask
	
	make method
		draw__maskXXXXX__Color__ColorLR()
		...
			
	add
		enum EFFECTGROUP
		enum EFFECTGROUP_DETAIL__XXXXX
		W_EffectGroup
			->Dice_EffectGroup() will be automatically changed.
			
		Dice_EffectGroup_Detail()
	
	add
		switch() in draw().
		switch() in print_EffectType : for debug.
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxHapPlayer.h"

#include "sj_common.h"


/************************************************************
class
************************************************************/

/**************************************************
**************************************************/
class EFFECT{
private:
	/****************************************
	enum
	****************************************/
	
	/********************
	********************/
	enum EFFECT_MIRRROR{
		EFFECT_MIRROR__NONE,
		EFFECT_MIRROR__LR,
		EFFECT_MIRROR__UP_DOWN,
		EFFECT_MIRROR__DIAG,
	};
	
	enum EFFECT_COLOR{
		EFFECT_COLOR__RGB,
		EFFECT_COLOR__GRAY,
		EFFECT_COLOR__GRAY_INV,
	};
	
	/********************
	modify with :FileName_mask
	********************/
	enum MASK_TYPE{
		MASK_DIAG,
		MASK_GEOM0,
		MASK_LR,
		MASK_LRLR,
		MASK_PYRAMID,
		MASK_UPDOWN,
		
		NUM_MASKS,
	};
	
	/********************
	********************/
	enum EFFECTGROUP{
		EFFECTGROUP__NONE,
		EFFECTGROUP__DIAG,
		EFFECTGROUP__GEOM0,
		EFFECTGROUP__LR,
		EFFECTGROUP__LRLR,
		EFFECTGROUP__PYRAMID,
		EFFECTGROUP__UPDOWN,
		
		NUM_EFFECTGROUP,
	};
	
	enum EFFECTGROUP_DETAIL__DIAG{
		MASK_DIAG__COLOR__COLOR_DIAG,
		MASK_DIAG__COLOR__GRAY_DIAG,
		MASK_DIAG__COLOR__GRAYINV_DIAG,
		MASK_DIAG__COLOR__GRAYINV,
		
		NUM_EFFECTGROUP_DETAIL__DIAG,
	};
	enum EFFECTGROUP_DETAIL__GEOM0{
		MASK_GEOM0__COLOR__COLOR_LR,
		MASK_GEOM0__COLOR__GRAY_LR,
		MASK_GEOM0__COLOR__GRAYINV_LR,
		MASK_GEOM0__COLOR__GRAYINV,
		
		NUM_EFFECTGROUP_DETAIL__GEOM0,
	};
	enum EFFECTGROUP_DETAIL__LR{
		MASK_LR__COLOR__COLOR_LR,
		MASK_LR__COLOR__GRAY_LR,
		MASK_LR__COLOR__GRAYINV_LR,
		
		NUM_EFFECTGROUP_DETAIL__LR,
	};
	enum EFFECTGROUP_DETAIL__LRLR{
		MASK_LRLR__COLOR__COLOR_LR,
		MASK_LRLR__COLOR__GRAY_LR,
		MASK_LRLR__COLOR__GRAYINV_LR,
		
		NUM_EFFECTGROUP_DETAIL__LRLR,
	};
	enum EFFECTGROUP_DETAIL__PYRAMID{
		MASK_PYRAMID__COLOR__COLOR_LR,
		MASK_PYRAMID__COLOR__GRAY_LR,
		MASK_PYRAMID__COLOR__GRAYINV_LR,
		MASK_PYRAMID__COLOR__GRAYINV,
		
		NUM_EFFECTGROUP_DETAIL__PYRAMID,
	};
	enum EFFECTGROUP_DETAIL__UPDOWN{
		MASK_UPDOWN__COLOR__COLOR_UPDOWN,
		MASK_UPDOWN__COLOR__GRAY_UPDOWN,
		MASK_UPDOWN__COLOR__GRAYINV_UPDOWN,
		
		NUM_EFFECTGROUP_DETAIL__UPDOWN,
	};
	
	/****************************************
	****************************************/
	/********************
	********************/
	static char FileName_mask[NUM_MASKS][BUF_SIZE];
	static int W_EffectGroup[NUM_EFFECTGROUP];
	
	bool b_Rev_EffectedArea_inShader;
	ofImage img_mask[NUM_MASKS];
	
	EFFECTGROUP EffectGroup;
	int EffectGroup_Detail;
	
	/********************
	********************/
	ofFbo fbo_org;
	ofFbo fbo_img0;
	ofFbo fbo_img1;
	
	/********************
	********************/
	ofShader shader_Gray;
	ofShader shader_Gray_Inv;
	ofShader shader_mix;

	
	/****************************************
	function
	****************************************/
	/********************
	singleton
	********************/
	EFFECT();
	~EFFECT();
	EFFECT(const EFFECT&); // Copy constructor. no define.
	EFFECT& operator=(const EFFECT&); // コピー代入演算子. no define.
	
	/********************
	********************/
	bool checkif_FileExist(const char* FileName);
	
	void Dice_EffectGroup();
	void Dice_EffectGroup_Detail();
	int Dice_index(int NUM);
	int Dice_index(int *Weight, int NUM);
	void print_EffectType();

	
	/********************
	********************/
	void DrawTool__Video_to_fbo(ofxHapPlayer& video, ofFbo& fbo);
	void DrawTool__fbo_mix(ofFbo& fbo_target, MASK_TYPE MaskType);
	void DrawTool__FboToFbo_ColorAndMirror(ofFbo& fbo_from, ofFbo& fbo_to, EFFECT_COLOR EffectColor, EFFECT_MIRRROR EffectMirror);
	
	/********************
	********************/
	void draw__Normal(ofFbo& fbo_target);
	void draw__maskLR__Color__ColorLR(ofFbo& fbo_target);
	void draw__maskLR__Color__GrayLR(ofFbo& fbo_target);
	void draw__maskLR__Color__GrayInvLR(ofFbo& fbo_target);
	void draw__maskLRLR__Color__ColorLR(ofFbo& fbo_target);
	void draw__maskLRLR__Color__GrayLR(ofFbo& fbo_target);
	void draw__maskLRLR__Color__GrayInvLR(ofFbo& fbo_target);
	void draw__maskUpDown__Color__ColorUpDown(ofFbo& fbo_target);
	void draw__maskUpDown__Color__GrayUpDown(ofFbo& fbo_target);
	void draw__maskUpDown__Color__GrayInvUpDown(ofFbo& fbo_target);
	void draw__maskDiag__Color__ColorDiag(ofFbo& fbo_target);
	void draw__maskDiag__Color__GrayDiag(ofFbo& fbo_target);
	void draw__maskDiag__Color__GrayInvDiag(ofFbo& fbo_target);
	void draw__maskDiag__Color__GrayInv(ofFbo& fbo_target);
	void draw__maskPyramid__Color__ColorLR(ofFbo& fbo_target);
	void draw__maskPyramid__Color__GrayLR(ofFbo& fbo_target);
	void draw__maskPyramid__Color__GrayInvLR(ofFbo& fbo_target);
	void draw__maskPyramid__Color__GrayInv(ofFbo& fbo_target);
	void draw__maskGeom0__Color__ColorLR(ofFbo& fbo_target);
	void draw__maskGeom0__Color__GrayLR(ofFbo& fbo_target);
	void draw__maskGeom0__Color__GrayInvLR(ofFbo& fbo_target);
	void draw__maskGeom0__Color__GrayInv(ofFbo& fbo_target);
	
public:
	/****************************************
	****************************************/
	/********************
	********************/
	static EFFECT* getInstance(){
		static EFFECT inst;
		return &inst;
	}
	
	void exit();
	void setup();
	void update();
	void draw(ofxHapPlayer& video, ofFbo& fbo_target);
	
	void Dice_Effect();
};


