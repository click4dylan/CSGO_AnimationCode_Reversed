#include "CreateMove.h"
#include "Animation.h"
#include "BaseEntity.h"
#include "CViewModel.h"
#include "CPlayerrecord.h"
#include "utlbuffer.h"
#include "GetValveAllocator.h"

#ifdef NO_PARALLEL_NETCODE
#define ANIMATION_MUTEX_LOCK Interfaces::MDLCache->BeginLock();
#define ANIMATION_MUTEX_UNLOCK Interfaces::MDLCache->EndLock();
#else
#define ANIMATION_MUTEX_LOCK ;
#define ANIMATION_MUTEX_UNLOCK ;
#endif

#include "Adriel/adr_util.hpp"

//server 0x2C4 bytes
#pragma pack(push, 1)
class CCSGOPlayerAnimState
{
public:
	int *m_pAnimLayerOrder;
	bool m_bIsReset;
	char firstpad[3];
	float m_flFlashedStartTime;
	float m_flFlashedEndTime;
	AimLayers m_AimLayers;
	int m_nModelIndex;
	CBaseEntity *pBaseEntity;
	CBaseCombatWeapon *pActiveWeapon;
	CBaseCombatWeapon *pLastActiveWeapon;
	float m_flLastClientSideAnimationUpdateTime;
	int32_t m_iLastClientSideAnimationUpdateFramecount;
	float m_flLastClientSideAnimationUpdateTimeDelta;
	float m_flEyeYaw;
	float m_flPitch;
	float m_flGoalFeetYaw;
	float m_flCurrentFeetYaw;
	float m_flCurrentMoveDirGoalFeetDelta;
	float m_flGoalMoveDirGoalFeetDelta;
	float m_flFeetVelDirDelta;
	float pad_0094;
	float m_flFeetCycle;
	float m_flFeetWeight;
	float m_fUnknown2;
	float m_fDuckAmount;
	float m_flHitGroundCycle;
	float m_fUnknown3;
	Vector m_vOrigin;
	Vector m_vLastOrigin;
	Vector m_vVelocity;
	Vector m_vVelocityNormalized;
	Vector m_vecLastAcceleratingVelocity;
	float m_flSpeed;
	float m_flAbsVelocityZ;
	float m_flSpeedNormalized;
	float m_flRunningSpeed;
	float m_flDuckingSpeed;
	float m_flTimeSinceStartedMoving;
	float m_flTimeSinceStoppedMoving;
	bool m_bOnGround;
	bool m_bJumping;
	char pad_010A[2];
	float m_flNextLowerBodyYawUpdateTime;
	bool m_bInParachute;
	bool m_bInHitGroundAnimation;
	char PAD_FUCKER[2];
	int m_iUnknownIntBlah;
	float m_flTotalTimeInAir;
	float m_flStartJumpZOrigin;
	float m_flHitGroundWeight;
	float m_flGroundFraction;
	bool m_bJust_Landed;
	bool m_bJust_LeftGround;
	char pad_0120[2];
	float m_flDuckRate;
	bool m_bOnLadder;
	char pad_0128[3];
	float m_flLadderCycle;
	float m_flLadderWeight;
	bool m_bNotRunning;
	bool m_bDefusing;
	bool m_bPlantingBomb;
	char pad_0135;
	bool m_bInBalanceAdjust;
	char pad_0141[3];
	CUtlVector<CUtlSymbol> m_ActivityModifiers;
	int gap148[1];
	float m_flTimeOfLastInjury;
	float m_flLastSetupLeanCurtime;
	Vector m_vecLastSetupLeanVelocity;
	Vector m_vecSetupLeanVelocityDelta;
	Vector m_vecSetupLeanVelocityInterpolated;
	float m_flLeanWeight;
	int m_iUnknownIntArr2[2];
	bool m_bFlashed;
	char m_bFlashedPad[3];
	float m_flStrafeWeight;
	int m_iUnknownint3;
	float m_flStrafeCycle;
	int m_iStrafeSequence;
	bool m_bStrafing;
	char m_bStrafingPad[3];
	float m_flTotalStrafeTime;
	int m_iUnknownInt4;
	bool m_bUnknownBool__;
	bool m_bIsAccelerating;
	char pad_01AE[2];
	animstate_pose_param_cache_t m_arrPoseParameters[20];
	bool m_bDeploying;
	char pad__[3];
	DWORD m_iUnknownInt__;
	float m_flGoalRunningSpeed;
	int m_iMoveState;
	float m_flMovePlaybackRate;
	float m_flUnknownFL0;
	float m_flMinYaw;
	float m_flMaxYaw;
	float m_flMinPitch;
	float m_flMaxPitch;
	int m_iAnimsetVersion;

	float m_flCurTime; //dylan added this so we can multithread the server side animation code instead of using gpGlobals->curtime

	CCSGOPlayerAnimState& operator=(const CCSGOPlayerAnimState &vOther);
	::CCSGOPlayerAnimState(CBaseEntity* ent);
	::CCSGOPlayerAnimState(const CCSGOPlayerAnimState &vOther);
	::CCSGOPlayerAnimState() {};
	void  Reset();
	bool  IsLayerSequenceFinished(int layer);
	void  SetLayerSequenceFromActivity(int layer, int activity);
	void  IncrementLayerCycle(int layer, bool is_looping);
	void  IncrementLayerCycleGeneric(int layer);
	void  IncrementLayerCycleWeightRateGeneric(int layer);
	void  LayerWeightAdvance(int layer);
	void  SetFeetCycle(float cycle);
	int   GetLayerActivity(int layer);
	int   GetWeightedSequenceFromActivity(int activity);
	void  Update(float yaw, float pitch);
	void  UpdateSetupVelocityAndSetupLean(float yaw, float pitch);
	void  SetupFlinch();
	void  SetupAliveloop();
	void  SetupFlashedReaction();
	void  SetupWholeBodyAction();
	void  SetupMovement();
	void  SetupLean();
	void  SetupWeaponAction();
	void  SetupAimMatrix();
	void  SetupVelocity();
	bool  CacheSequences();
	void  SetLayerPlaybackRate(int layer, float playbackrate);
	void  SetLayerCycle(int layer, float cycle);
	void  SetLayerCycleDirect(int layer, float cycle);
	void  SetLayerWeight(int layer, float weight);
	void  SetLayerWeightDeltaRate(int layer, float oldweight);
	void  SetLayerSequence(int layer, int sequence);
	float GetLayerCycle(int layer);
	float GetLayerWeight(int layer);
	char* GetWeaponPrefix();
	void  UpdateAnimLayer(float playbackrate, int layer, int sequence, float weight, float cycle);
	void  UpdateLayerOrderPreset(float weight, int layer, int sequence);
	float GetLayerIdealWeightFromSeqCycle(int layer);
	void  ModifyEyePosition(Vector& vecEyePos);
	void  UpdateAimLayer(AimLayer *layer, float timedelta, float multiplier, bool somebool);
	void  DoAnimationEvent(int event);
	void  ApplyLayerOrderPreset(int* order, bool force_apply);
	bool  LayerSequenceHasActMod(int layer, char* sequence);
	int   GetLayerSequence(int layer);
	C_AnimationLayer* GetAnimOverlay(int layer);
	void AddActivityModifier(const char *szName);
	void UpdateActivityModifiers();
};
#pragma pack(pop)

char *animtaguninitializedstr = new char[22]{ 59, 52, 51, 55, 46, 59, 61, 37, 47, 52, 51, 52, 51, 46, 51, 59, 54, 51, 32, 63, 62, 0 }; /*ANIMTAG_UNINITIALIZED*/
char *animtagstartcyclenstr = new char[21]{ 59, 52, 51, 55, 46, 59, 61, 37, 41, 46, 59, 40, 46, 57, 35, 57, 54, 63, 37, 52, 0 }; /*ANIMTAG_STARTCYCLE_N*/
char *animtagstartcyclenestr = new char[22]{ 59, 52, 51, 55, 46, 59, 61, 37, 41, 46, 59, 40, 46, 57, 35, 57, 54, 63, 37, 52, 63, 0 }; /*ANIMTAG_STARTCYCLE_NE*/
char *animtagstartcycleestr = new char[21]{ 59, 52, 51, 55, 46, 59, 61, 37, 41, 46, 59, 40, 46, 57, 35, 57, 54, 63, 37, 63, 0 }; /*ANIMTAG_STARTCYCLE_E*/
char *animtagstartcyclesestr = new char[22]{ 59, 52, 51, 55, 46, 59, 61, 37, 41, 46, 59, 40, 46, 57, 35, 57, 54, 63, 37, 41, 63, 0 }; /*ANIMTAG_STARTCYCLE_SE*/
char *animtagstartcyclesstr = new char[21]{ 59, 52, 51, 55, 46, 59, 61, 37, 41, 46, 59, 40, 46, 57, 35, 57, 54, 63, 37, 41, 0 }; /*ANIMTAG_STARTCYCLE_S*/
char *animtagstartcycleswstr = new char[22]{ 59, 52, 51, 55, 46, 59, 61, 37, 41, 46, 59, 40, 46, 57, 35, 57, 54, 63, 37, 41, 45, 0 }; /*ANIMTAG_STARTCYCLE_SW*/
char *animtagstartcyclewstr = new char[21]{ 59, 52, 51, 55, 46, 59, 61, 37, 41, 46, 59, 40, 46, 57, 35, 57, 54, 63, 37, 45, 0 }; /*ANIMTAG_STARTCYCLE_W*/
char *animtagstartcyclenwstr = new char[22]{ 59, 52, 51, 55, 46, 59, 61, 37, 41, 46, 59, 40, 46, 57, 35, 57, 54, 63, 37, 52, 45, 0 }; /*ANIMTAG_STARTCYCLE_NW*/
char *animtagaimlimityawminidlestr = new char[29]{ 59, 52, 51, 55, 46, 59, 61, 37, 59, 51, 55, 54, 51, 55, 51, 46, 37, 35, 59, 45, 55, 51, 52, 37, 51, 62, 54, 63, 0 }; /*ANIMTAG_AIMLIMIT_YAWMIN_IDLE*/
char *animtagaimlimityawmaxidlestr = new char[29]{ 59, 52, 51, 55, 46, 59, 61, 37, 59, 51, 55, 54, 51, 55, 51, 46, 37, 35, 59, 45, 55, 59, 34, 37, 51, 62, 54, 63, 0 }; /*ANIMTAG_AIMLIMIT_YAWMAX_IDLE*/
char *animtagaimlimityawminwalkstr = new char[29]{ 59, 52, 51, 55, 46, 59, 61, 37, 59, 51, 55, 54, 51, 55, 51, 46, 37, 35, 59, 45, 55, 51, 52, 37, 45, 59, 54, 49, 0 }; /*ANIMTAG_AIMLIMIT_YAWMIN_WALK*/
char *animtagaimlimityawmaxwalkstr = new char[29]{ 59, 52, 51, 55, 46, 59, 61, 37, 59, 51, 55, 54, 51, 55, 51, 46, 37, 35, 59, 45, 55, 59, 34, 37, 45, 59, 54, 49, 0 }; /*ANIMTAG_AIMLIMIT_YAWMAX_WALK*/
char *animtagaimlimityawminrunstr = new char[28]{ 59, 52, 51, 55, 46, 59, 61, 37, 59, 51, 55, 54, 51, 55, 51, 46, 37, 35, 59, 45, 55, 51, 52, 37, 40, 47, 52, 0 }; /*ANIMTAG_AIMLIMIT_YAWMIN_RUN*/
char *animtagaimlimityawmaxrunstr = new char[28]{ 59, 52, 51, 55, 46, 59, 61, 37, 59, 51, 55, 54, 51, 55, 51, 46, 37, 35, 59, 45, 55, 59, 34, 37, 40, 47, 52, 0 }; /*ANIMTAG_AIMLIMIT_YAWMAX_RUN*/
char *animtagaimlimityawmincrouchidlestr = new char[35]{ 59, 52, 51, 55, 46, 59, 61, 37, 59, 51, 55, 54, 51, 55, 51, 46, 37, 35, 59, 45, 55, 51, 52, 37, 57, 40, 53, 47, 57, 50, 51, 62, 54, 63, 0 }; /*ANIMTAG_AIMLIMIT_YAWMIN_CROUCHIDLE*/
char *animtagaimlimityawmaxcrouchidlestr = new char[35]{ 59, 52, 51, 55, 46, 59, 61, 37, 59, 51, 55, 54, 51, 55, 51, 46, 37, 35, 59, 45, 55, 59, 34, 37, 57, 40, 53, 47, 57, 50, 51, 62, 54, 63, 0 }; /*ANIMTAG_AIMLIMIT_YAWMAX_CROUCHIDLE*/
char *animtagaimlimityawmincrouchwalkstr = new char[35]{ 59, 52, 51, 55, 46, 59, 61, 37, 59, 51, 55, 54, 51, 55, 51, 46, 37, 35, 59, 45, 55, 51, 52, 37, 57, 40, 53, 47, 57, 50, 45, 59, 54, 49, 0 }; /*ANIMTAG_AIMLIMIT_YAWMIN_CROUCHWALK*/
char *animtagaimlimityawmaxcrouchwalkstr = new char[35]{ 59, 52, 51, 55, 46, 59, 61, 37, 59, 51, 55, 54, 51, 55, 51, 46, 37, 35, 59, 45, 55, 59, 34, 37, 57, 40, 53, 47, 57, 50, 45, 59, 54, 49, 0 }; /*ANIMTAG_AIMLIMIT_YAWMAX_CROUCHWALK*/
char *animtagaimlimitpitchminidlestr = new char[31]{ 59, 52, 51, 55, 46, 59, 61, 37, 59, 51, 55, 54, 51, 55, 51, 46, 37, 42, 51, 46, 57, 50, 55, 51, 52, 37, 51, 62, 54, 63, 0 }; /*ANIMTAG_AIMLIMIT_PITCHMIN_IDLE*/
char *animtagaimlimitpitchmaxidlestr = new char[31]{ 59, 52, 51, 55, 46, 59, 61, 37, 59, 51, 55, 54, 51, 55, 51, 46, 37, 42, 51, 46, 57, 50, 55, 59, 34, 37, 51, 62, 54, 63, 0 }; /*ANIMTAG_AIMLIMIT_PITCHMAX_IDLE*/
char *animtagaimlimitpitchminwalkrunstr = new char[34]{ 59, 52, 51, 55, 46, 59, 61, 37, 59, 51, 55, 54, 51, 55, 51, 46, 37, 42, 51, 46, 57, 50, 55, 51, 52, 37, 45, 59, 54, 49, 40, 47, 52, 0 }; /*ANIMTAG_AIMLIMIT_PITCHMIN_WALKRUN*/
char *animtagaimlimitpitchmaxwalkrunstr = new char[34]{ 59, 52, 51, 55, 46, 59, 61, 37, 59, 51, 55, 54, 51, 55, 51, 46, 37, 42, 51, 46, 57, 50, 55, 59, 34, 37, 45, 59, 54, 49, 40, 47, 52, 0 }; /*ANIMTAG_AIMLIMIT_PITCHMAX_WALKRUN*/
char *animtagaimlimitpitchmincrouchstr = new char[33]{ 59, 52, 51, 55, 46, 59, 61, 37, 59, 51, 55, 54, 51, 55, 51, 46, 37, 42, 51, 46, 57, 50, 55, 51, 52, 37, 57, 40, 53, 47, 57, 50, 0 }; /*ANIMTAG_AIMLIMIT_PITCHMIN_CROUCH*/
char *animtagaimlimitpitchmaxcrouchstr = new char[33]{ 59, 52, 51, 55, 46, 59, 61, 37, 59, 51, 55, 54, 51, 55, 51, 46, 37, 42, 51, 46, 57, 50, 55, 59, 34, 37, 57, 40, 53, 47, 57, 50, 0 }; /*ANIMTAG_AIMLIMIT_PITCHMAX_CROUCH*/
char *animtagaimlimitpitchmincrouchwalkstr = new char[37]{ 59, 52, 51, 55, 46, 59, 61, 37, 59, 51, 55, 54, 51, 55, 51, 46, 37, 42, 51, 46, 57, 50, 55, 51, 52, 37, 57, 40, 53, 47, 57, 50, 45, 59, 54, 49, 0 }; /*ANIMTAG_AIMLIMIT_PITCHMIN_CROUCHWALK*/
char *animtagaimlimitpitchmaxcrouchwalkstr = new char[37]{ 59, 52, 51, 55, 46, 59, 61, 37, 59, 51, 55, 54, 51, 55, 51, 46, 37, 42, 51, 46, 57, 50, 55, 59, 34, 37, 57, 40, 53, 47, 57, 50, 45, 59, 54, 49, 0 }; /*ANIMTAG_AIMLIMIT_PITCHMAX_CROUCHWALK*/
char *animtagflashbangpassablestr = new char[27]{ 59, 52, 51, 55, 46, 59, 61, 37, 60, 54, 59, 41, 50, 56, 59, 52, 61, 37, 42, 59, 41, 41, 59, 56, 54, 63, 0 }; /*ANIMTAG_FLASHBANG_PASSABLE*/
char *animtagweaponpostlayerstr = new char[25]{ 59, 52, 51, 55, 46, 59, 61, 37, 45, 63, 59, 42, 53, 52, 37, 42, 53, 41, 46, 54, 59, 35, 63, 40, 0 }; /*ANIMTAG_WEAPON_POSTLAYER*/

inline int GetTagIndexFromAnimTagName(const char *name)
{
#ifndef NO_PARALLEL_NETCODE
	Interfaces::MDLCache->BeginLock();
#endif
	for (animtag_desc_t* pTag = &g_AnimTags[1]; pTag < &g_AnimTags[ANIMTAG_MAX]; ++pTag)
	{
		DecStr(pTag->name);
		if (!strcmp(name, pTag->name))
		{
			EncStr(pTag->name);
#ifndef NO_PARALLEL_NETCODE
			Interfaces::MDLCache->EndLock();
#endif
			return pTag->index;
		}
		EncStr(pTag->name);
	}
#ifndef NO_PARALLEL_NETCODE
	Interfaces::MDLCache->EndLock();
#endif
	return -1;
}

animtag_desc_t g_AnimTags[ANIMTAG_MAX] =
{
	{ANIMTAG_UNINITIALIZED,					animtaguninitializedstr},
	{ANIMTAG_STARTCYCLE_N,					animtagstartcyclenstr},
	{ANIMTAG_STARTCYCLE_NE, 				animtagstartcyclenestr},
	{ANIMTAG_STARTCYCLE_E,					animtagstartcycleestr},
	{ANIMTAG_STARTCYCLE_SE,					animtagstartcyclesestr},
	{ANIMTAG_STARTCYCLE_S,					animtagstartcyclesstr},
	{ANIMTAG_STARTCYCLE_SW,					animtagstartcycleswstr},
	{ANIMTAG_STARTCYCLE_W,					animtagstartcyclewstr},
	{ANIMTAG_STARTCYCLE_NW,					animtagstartcyclenwstr},
	{ANIMTAG_AIMLIMIT_YAWMIN_IDLE,			animtagaimlimityawminidlestr},
	{ANIMTAG_AIMLIMIT_YAWMAX_IDLE,			animtagaimlimityawmaxidlestr},
	{ANIMTAG_AIMLIMIT_YAWMIN_WALK,			animtagaimlimityawminwalkstr},
	{ANIMTAG_AIMLIMIT_YAWMAX_WALK,			animtagaimlimityawmaxwalkstr},
	{ANIMTAG_AIMLIMIT_YAWMIN_RUN,			animtagaimlimityawminrunstr},
	{ANIMTAG_AIMLIMIT_YAWMAX_RUN,			animtagaimlimityawmaxrunstr},
	{ANIMTAG_AIMLIMIT_YAWMIN_CROUCHIDLE,	animtagaimlimityawmincrouchidlestr},
	{ANIMTAG_AIMLIMIT_YAWMAX_CROUCHIDLE,	animtagaimlimityawmaxcrouchidlestr},
	{ANIMTAG_AIMLIMIT_YAWMIN_CROUCHWALK,	animtagaimlimityawmincrouchwalkstr},
	{ANIMTAG_AIMLIMIT_YAWMAX_CROUCHWALK,	animtagaimlimityawmaxcrouchwalkstr},
	{ANIMTAG_AIMLIMIT_PITCHMIN_IDLE,		animtagaimlimitpitchminidlestr},
	{ANIMTAG_AIMLIMIT_PITCHMAX_IDLE,		animtagaimlimitpitchmaxidlestr},
	{ANIMTAG_AIMLIMIT_PITCHMIN_WALKRUN,		animtagaimlimitpitchminwalkrunstr},
	{ANIMTAG_AIMLIMIT_PITCHMAX_WALKRUN,		animtagaimlimitpitchmaxwalkrunstr},
	{ANIMTAG_AIMLIMIT_PITCHMIN_CROUCH,		animtagaimlimitpitchmincrouchstr},
	{ANIMTAG_AIMLIMIT_PITCHMAX_CROUCH,		animtagaimlimitpitchmaxcrouchstr},
	{ANIMTAG_AIMLIMIT_PITCHMIN_CROUCHWALK,	animtagaimlimitpitchmincrouchwalkstr},
	{ANIMTAG_AIMLIMIT_PITCHMAX_CROUCHWALK,	animtagaimlimitpitchmaxcrouchwalkstr},
	{ANIMTAG_FLASHBANG_PASSABLE,			animtagflashbangpassablestr},
	{ANIMTAG_WEAPON_POSTLAYER,				animtagweaponpostlayerstr}
};

//server
//size 0x5C
struct __declspec(align(4)) CAnimationLayer
{
	int m_nFlags;
	bool m_bSequenceFinished;
	bool m_bLooping;
	char pad_0x0006[2];
	int m_nSequence;
	float m_flCycle;
	float m_flPlaybackRate;
	char pad_0x0014[4];
	float m_flWeight;
	float m_flWeightDeltaRate;
	char pad_0x001C[24];
	int _m_fFlags;
	int m_iActivity;
	int m_nPriority;
	char pad_0x001C_2[8];
	int m_nOrder;
	float m_flLastEventCheck;
	float m_flLastAccess;
	CBaseEntity* m_pOwnerEntity;
};


void CBaseEntity::UpdateServerSideAnimation(int mode, float curtime, bool _RunOnlySetupVelocitySetupLean)
{
	if (IsCustomPlayer())
	{
		CPlayerrecord* playerrecord = ToPlayerRecord();
		CCSGOPlayerAnimState* animstate = ToPlayerRecord()->m_pAnimStateServer[mode];

		animstate->m_flCurTime = curtime;
		if (!_RunOnlySetupVelocitySetupLean)
			animstate->Update(EyeAngles()->y, EyeAngles()->x);
		else
			animstate->UpdateSetupVelocityAndSetupLean(EyeAngles()->y, EyeAngles()->x);

		if (mode != 0 || _RunOnlySetupVelocitySetupLean)
			return;
	}
	else
	{
		if (GetSequence() != -1)
			FrameAdvance(0);

		if (IsLocalPlayer())
			((CCSGOPlayerAnimState*)GetBasePlayerAnimState())->Update(EyeAngles()->y, EyeAngles()->x);
		else
			((CCSGOPlayerAnimState*)GetBasePlayerAnimState())->Update(GetEyeAngles().y, GetEyeAngles().x);
	}

	if (GetSequence() != -1)
		OnLatchInterpolatedVariables(1);

	if (WasKilledByTaser())
		HandleTaserAnimation();

	if (IsLocalPlayer())
	{
		CBaseCombatWeapon *weapon = GetActiveCSWeapon();
		if (weapon)
		{
			C_BaseViewModel *viewmodel = GetViewModel(GetViewModelIndex());
			if (viewmodel)
				viewmodel->UpdateAllViewmodelAddons();
		}
		else
		{
			for (int i = 0; i < 3; ++i)
			{
				C_BaseViewModel *viewmodel = GetViewModel(i);
				if (viewmodel)
				{
					viewmodel->RemoveViewmodelArmModels();
					viewmodel->RemoveViewmodelLabel();
					viewmodel->RemoveViewmodelStatTrak();
					viewmodel->RemoveViewmodelStickers();
				}
			}
		}
	}
}

int s_animLayerOrderDefault[13] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
int s_animLayerOrderWeaponPost[13] = { 0, 2, 3, 4, 5, 6, 7, 1, 8, 9, 10, 11, 12 };
char *knifestr = new char[6]{ 17, 20, 19, 28, 31, 0 }; /*knife*/
char *pistolstr = new char[7]{ 10, 19, 9, 14, 21, 22, 0 }; /*pistol*/
char *smgstr = new char[4]{ 9, 23, 29, 0 }; /*smg*/
char *riflestr = new char[6]{ 8, 19, 28, 22, 31, 0 }; /*rifle*/
char *shotgunstr = new char[8]{ 9, 18, 21, 14, 29, 15, 20, 0 }; /*shotgun*/
char *sniperstr = new char[7]{ 9, 20, 19, 10, 31, 8, 0 }; /*sniper*/
char *heavystr = new char[6]{ 18, 31, 27, 12, 3, 0 }; /*heavy*/
char *c4str = new char[3]{ 25, 78, 0 }; /*c4*/
char *pistol2str = new char[7]{ 10, 19, 9, 14, 21, 22, 0 }; /*pistol*/
char *grenadestr = new char[8]{ 29, 8, 31, 20, 27, 30, 31, 0 }; /*grenade*/
char *knife2str = new char[6]{ 17, 20, 19, 28, 31, 0 }; /*knife*/
char *g_szWeaponPrefixLookupTable[11] = { knifestr, pistolstr, smgstr, riflestr, shotgunstr, sniperstr, heavystr, c4str, pistol2str, grenadestr, knife2str };

CCSGOPlayerAnimState* CreateCSGOPlayerAnimState(CBaseEntity* ent)
{
	CCSPlayer *player = nullptr;
	if (ent->IsPlayer())
		player = ent;//ToCSPlayer(ent);

	return new CCSGOPlayerAnimState(player);
}

CUtlSymbolTable *g_ActivityModifiersTable = nullptr;

void CCSGOPlayerAnimState::AddActivityModifier(const char *szName)
{
	char name[32];
	V_strncpy(name, szName, 32);
	CUtlSymbol sym = g_ActivityModifiersTable->Find(name);
	if (!sym.IsValid())
	{
		sym = g_ActivityModifiersTable->AddString(name);
	}
	m_ActivityModifiers.AddToTail(sym);
}

void CCSGOPlayerAnimState::UpdateActivityModifiers()
{
	m_ActivityModifiers.Purge();

	AddActivityModifier(GetWeaponPrefix());

#ifndef NO_PARALLEL_NETCODE
	Interfaces::MDLCache->BeginLock();
#endif
	if (m_flRunningSpeed > 0.25f)
	{
		//decrypts(0)
		AddActivityModifier(XorStr("moving"));
		//encrypts(0)
	}
	if (m_fDuckAmount > 0.55f)
	{
		//decrypts(0)
		AddActivityModifier(XorStr("crouch"));
		//encrypts(0)
	}
#ifndef NO_PARALLEL_NETCODE
	Interfaces::MDLCache->EndLock();
#endif
}

CCSGOPlayerAnimState::CCSGOPlayerAnimState(CBaseEntity* ent)
{
	for (int i = 0; i < 3; ++i)
	{
		m_AimLayers.layers[i].m_flUnknown0 = 0.0f;
		m_AimLayers.layers[i].m_flTotalTime = 0.0f;
		m_AimLayers.layers[i].m_flUnknown1 = 0.3f;
		m_AimLayers.layers[i].m_flUnknown2 = 0.3f;
		m_AimLayers.layers[i].m_flWeight = 0.0f;
	}
	
	m_ActivityModifiers.Purge();

	for (int i = 0; i < 20; ++i)
	{
		m_arrPoseParameters[i].m_bInitialized = false;
		m_arrPoseParameters[i].m_iPoseParameter = -1;
		m_arrPoseParameters[i].m_szPoseParameter = "";
	}

	pBaseEntity = ent;
	m_nModelIndex = -1;
	m_iAnimsetVersion = 2;
	Reset();
}

CCSGOPlayerAnimState& CCSGOPlayerAnimState::operator=(const CCSGOPlayerAnimState &vOther)
{
	DWORD firstsz = (DWORD)&this->m_ActivityModifiers - (DWORD)this;
	DWORD secondsz = ((DWORD)this + sizeof(CCSGOPlayerAnimState)) - (DWORD)&this->gap148;
	memcpy(this, &vOther, firstsz);
	memcpy(&gap148, &vOther.gap148, secondsz);
	m_ActivityModifiers = vOther.m_ActivityModifiers;
	return *this;
}

CCSGOPlayerAnimState::CCSGOPlayerAnimState(const CCSGOPlayerAnimState &vOther)
{
	DWORD firstsz = (DWORD)&this->m_ActivityModifiers - (DWORD)this;
	DWORD secondsz = ((DWORD)this + sizeof(CCSGOPlayerAnimState)) - (DWORD)&this->gap148;
	memcpy(this, &vOther, firstsz);
	memcpy(&gap148, &vOther.gap148, secondsz);
	m_ActivityModifiers = vOther.m_ActivityModifiers;
}

void CCSGOPlayerAnimState::Reset()
{
	pBaseEntity->SetNumAnimOverlays(13);
	ApplyLayerOrderPreset(s_animLayerOrderDefault, true);
	m_flFlashedStartTime = 0.0f;
	m_flFlashedEndTime = 0.0f;
	CBaseCombatWeapon* weap = pBaseEntity->GetActiveCSWeapon();
	pActiveWeapon = weap;
	pLastActiveWeapon = weap;
	m_flLastClientSideAnimationUpdateTime = 0.0f;
	m_flLastClientSideAnimationUpdateTimeDelta = 0.0f;
	m_flEyeYaw = 0.0f;
	m_flPitch = 0.0f;
	m_flGoalFeetYaw = 0.0f;
	m_flCurrentFeetYaw = 0.0f;
	m_flCurrentMoveDirGoalFeetDelta = 0.0f;
	m_flGoalMoveDirGoalFeetDelta = 0.0f;
	m_flFeetVelDirDelta = 0.0f;
	if (pBaseEntity->IsLocalPlayer()) //dylan added this check! the server tells us the lby so we don't want to cause issues by doing this
	{
		if (pBaseEntity->GetLowerBodyYaw() != 0.0f)
			pBaseEntity->SetLowerBodyYaw(0.0f);
	}
	m_flNextLowerBodyYawUpdateTime = 0.0f;
	m_AimLayers.layers[0].m_flUnknown0 = 0.0f;
	m_AimLayers.layers[0].m_flTotalTime = 0.0f;
	m_AimLayers.layers[0].m_flUnknown1 = 0.3f;
	m_AimLayers.layers[0].m_flUnknown2 = 0.3f;
	m_AimLayers.layers[0].m_flWeight = 0.0f;
	m_AimLayers.layers[0].m_flUnknown1 = 0.4f;
	m_AimLayers.layers[0].m_flUnknown2 = 0.2f;
	m_AimLayers.layers[1].m_flUnknown0 = 0.0f;
	m_AimLayers.layers[1].m_flTotalTime = 0.0f;
	m_AimLayers.layers[1].m_flUnknown1 = 0.3f;
	m_AimLayers.layers[1].m_flUnknown2 = 0.3f;
	m_AimLayers.layers[1].m_flWeight = 0.0f;
	m_AimLayers.layers[1].m_flUnknown1 = 0.2f;
	m_AimLayers.layers[1].m_flUnknown2 = 0.4f;
	m_AimLayers.layers[2].m_flUnknown0 = 0.0f;
	m_AimLayers.layers[2].m_flTotalTime = 0.0f;
	m_AimLayers.layers[2].m_flUnknown1 = 0.3f;
	m_AimLayers.layers[2].m_flUnknown2 = 0.3f;
	m_AimLayers.layers[2].m_flWeight = 0.0f;
	m_AimLayers.layers[2].m_flUnknown1 = 0.3f;
	m_AimLayers.layers[2].m_flUnknown2 = 0.3f;
	m_flFeetCycle = 0.0f;
	m_flFeetWeight = 0.0f;
	m_fUnknown2 = 0.0f;
	m_fDuckAmount = 0.0f;
	m_flHitGroundCycle = 0.0f;
	m_fUnknown3 = 0.0f;
	m_vOrigin.x = 0.0f;
	m_vOrigin.y = 0.0f;
	m_vOrigin.z = 0.0f;
	m_vLastOrigin.x = 0.0f;
	m_vLastOrigin.y = 0.0f;
	m_vLastOrigin.z = 0.0f;
	m_vVelocity.x = 0.0f;
	m_vVelocity.y = 0.0f;
	m_vVelocity.z = 0.0f;
	m_vVelocityNormalized.x = 0.0f;
	m_vVelocityNormalized.y = 0.0f;
	m_vVelocityNormalized.z = 0.0f;
	m_vecLastAcceleratingVelocity.x = 0.0f;
	m_vecLastAcceleratingVelocity.y = 0.0f;
	m_vecLastAcceleratingVelocity.z = 0.0f;
	m_flSpeed = 0.0f;
	m_flAbsVelocityZ = 0.0f;
	m_flSpeedNormalized = 0.0f;
	m_flRunningSpeed = 0.0f;
	m_flDuckingSpeed = 0.0f;
	m_flTimeSinceStartedMoving = 0.0f;
	m_flTimeSinceStoppedMoving = 0.0f;
	m_bOnGround = 1;
	m_bInParachute = 0;
	m_flHitGroundWeight = 1.0f;
	m_flStartJumpZOrigin = 0.0f;
	m_iUnknownIntBlah = 0;
	m_flTotalTimeInAir = 0.0f;
	m_flGroundFraction = 0.0f;
	m_bJust_Landed = 0;
	m_flDuckRate = 0.0f;
	m_bOnLadder = 0;
	m_flLadderCycle = 0.0f;
	m_flLadderWeight = 0.0f;
	m_bNotRunning = 0;
	m_bInBalanceAdjust = 0;
	gap148[0] = 0;
	m_flTimeOfLastInjury = 0.0f;
	m_flLastSetupLeanCurtime = 0.0f;
	m_vecLastSetupLeanVelocity.x = 0.0f;
	m_vecLastSetupLeanVelocity.y = 0.0f;
	m_vecLastSetupLeanVelocity.z = 0.0f;
	m_vecSetupLeanVelocityDelta.x = 0.0f;
	m_vecSetupLeanVelocityDelta.y = 0.0f;
	m_vecSetupLeanVelocityDelta.z = 0.0f;
	m_vecSetupLeanVelocityInterpolated.x = 0.0f;
	m_vecSetupLeanVelocityInterpolated.y = 0.0f;
	m_vecSetupLeanVelocityInterpolated.z = 0.0f;
	m_flLeanWeight = 0.0f;
	m_iUnknownIntArr2[0] = 0;
	m_iUnknownIntArr2[1] = 0;
	m_bFlashed = 0;
	m_flStrafeWeight = 0.0f;
	m_iUnknownint3 = 0;
	m_flStrafeCycle = 0.0f;
	m_iStrafeSequence = -1;
	m_bStrafing = 0;
	m_flTotalStrafeTime = 0.0f;
	m_iUnknownInt4 = 0;
	m_bUnknownBool__ = 0;
	m_bDeploying = 0;
	m_iUnknownInt__ = 0;
	m_flGoalRunningSpeed = 80.0f;
	m_flMovePlaybackRate = 0.0f;
	m_iMoveState = 0;
	m_flUnknownFL0 = 0.0f;
	m_flMinYaw = -58.0f;
	m_flMaxYaw = 58.0f;
	m_flMinPitch = -90.0f;
	m_flMaxPitch = 90.0f;
	m_ActivityModifiers.Purge();
	m_bIsReset = 1;
}

void CCSGOPlayerAnimState::ApplyLayerOrderPreset(int* order, bool force_apply)
{
	if (force_apply || m_pAnimLayerOrder != order)
	{
		m_pAnimLayerOrder = order;
		for (int i = 0; i < 13; ++i)
		{
			int numanimoverlays = pBaseEntity->GetNumAnimOverlays();
			int iOrder = m_pAnimLayerOrder[i];
			if (iOrder > numanimoverlays - 1)
				iOrder = numanimoverlays - 1;

			if (numanimoverlays)
			{
				C_AnimationLayer *layer = pBaseEntity->GetAnimOverlay(iOrder);
				if (layer)
				{
					if (layer->m_nOrder != i)
						layer->SetOrder(i);
					layer->_m_fFlags = 0;
					layer->m_iActivity = -1;
					layer->m_iPriority = -1;
				}
			}
		}
	}
}

char *leanyawstr2 = new char[9]{ 22, 31, 27, 20, 37, 3, 27, 13, 0 }; /*lean_yaw*/
char *speedstr2 = new char[6]{ 9, 10, 31, 31, 30, 0 }; /*speed*/
char *ladderspeedstr2 = new char[13]{ 22, 27, 30, 30, 31, 8, 37, 9, 10, 31, 31, 30, 0 }; /*ladder_speed*/
char *ladderyawstr2 = new char[11]{ 22, 27, 30, 30, 31, 8, 37, 3, 27, 13, 0 }; /*ladder_yaw*/
char *moveyawstr2 = new char[9]{ 23, 21, 12, 31, 37, 3, 27, 13, 0 }; /*move_yaw*/
char *runstr2 = new char[4]{ 8, 15, 20, 0 }; /*run*/
char *bodyyawstr2 = new char[9]{ 24, 21, 30, 3, 37, 3, 27, 13, 0 }; /*body_yaw*/
char *bodypitchstr2 = new char[11]{ 24, 21, 30, 3, 37, 10, 19, 14, 25, 18, 0 }; /*body_pitch*/
char *deathyawstr2 = new char[10]{ 30, 31, 27, 14, 18, 37, 3, 27, 13, 0 }; /*death_yaw*/
char *standstr2 = new char[6]{ 9, 14, 27, 20, 30, 0 }; /*stand*/
char *jumpfallstr2 = new char[10]{ 16, 15, 23, 10, 37, 28, 27, 22, 22, 0 }; /*jump_fall*/
char *aimblendstandidlestr2 = new char[21]{ 27, 19, 23, 37, 24, 22, 31, 20, 30, 37, 9, 14, 27, 20, 30, 37, 19, 30, 22, 31, 0 }; /*aim_blend_stand_idle*/
char *aimblendcrouchidlestr2 = new char[22]{ 27, 19, 23, 37, 24, 22, 31, 20, 30, 37, 25, 8, 21, 15, 25, 18, 37, 19, 30, 22, 31, 0 }; /*aim_blend_crouch_idle*/
char *str2afeyawstr2 = new char[11]{ 9, 14, 8, 27, 28, 31, 37, 3, 27, 13, 0 }; /*strafe_yaw*/
char *aimblendstandwalkstr2 = new char[21]{ 27, 19, 23, 37, 24, 22, 31, 20, 30, 37, 9, 14, 27, 20, 30, 37, 13, 27, 22, 17, 0 }; /*aim_blend_stand_walk*/
char *aimblendstandrunstr2 = new char[20]{ 27, 19, 23, 37, 24, 22, 31, 20, 30, 37, 9, 14, 27, 20, 30, 37, 8, 15, 20, 0 }; /*aim_blend_stand_run*/
char *aimblendcrouchwalkstr2 = new char[22]{ 27, 19, 23, 37, 24, 22, 31, 20, 30, 37, 25, 8, 21, 15, 25, 18, 37, 13, 27, 22, 17, 0 }; /*aim_blend_crouch_walk*/
char *moveblendwalkstr2 = new char[16]{ 23, 21, 12, 31, 37, 24, 22, 31, 20, 30, 37, 13, 27, 22, 17, 0 }; /*move_blend_walk*/
char *moveblendrunstr2 = new char[15]{ 23, 21, 12, 31, 37, 24, 22, 31, 20, 30, 37, 8, 15, 20, 0 }; /*move_blend_run*/
char *moveblendcrouchstr2 = new char[18]{ 23, 21, 12, 31, 37, 24, 22, 31, 20, 30, 37, 25, 8, 21, 15, 25, 18, 0 }; /*move_blend_crouch*/
bool _didDecrypt2 = false;

bool CCSGOPlayerAnimState::CacheSequences()
{
	if (!pBaseEntity)
		return false;

	if (m_nModelIndex != pBaseEntity->GetModelIndex())
	{
		m_iAnimsetVersion = 0;

#if defined NO_MALLOC_OVERRIDE || defined NO_MEMOVERRIDE_NEW_DELETE
		m_iAnimsetVersion = 2;
#else
#ifndef NO_PARALLEL_NETCODE
		Interfaces::MDLCache->BeginLock();
#endif
		CUtlBuffer buffer(1024, 0, CUtlBuffer::TEXT_BUFFER);

		//decrypts(0)
		buffer.PutString(XorStr("keyvalues {\n"));
		//encrypts(0)
#ifndef NO_PARALLEL_NETCODE
		Interfaces::MDLCache->EndLock();
#endif
		if (Interfaces::ModelInfoClient->GetModelKeyValue(pBaseEntity->GetModel(), buffer))
		{
			buffer.PutString("\n}");
#if defined NO_MALLOC_OVERRIDE || defined NO_MEMOVERRIDE_NEW_DELETE
			KeyValues* keyValues = (KeyValues*)MALLOC(sizeof(KeyValues));
			keyValues->CallConstructor("");
#else
			auto keyValues = new KeyValues("");
#endif
			if (keyValues)
			{
				if (keyValues->LoadFromBuffer(Interfaces::ModelInfoClient->GetModelName(pBaseEntity->GetModel()), buffer.String()))
				{
#ifndef NO_PARALLEL_NETCODE
					Interfaces::MDLCache->BeginLock();
#endif
					//decrypts(0)
					for (KeyValues *sub = keyValues->GetFirstSubKey(); sub != NULL; sub = sub->GetNextKey())
					{
						if (sub->FindKey(XorStr("animset_version"), false))
						{
							m_iAnimsetVersion = sub->GetInt(XorStr("animset_version"), 2);
							break;
						}
					}
					//encrypts(0)
#ifndef NO_PARALLEL_NETCODE
					Interfaces::MDLCache->EndLock();
#endif
				}

				keyValues->deleteThis();
			}
		}

#endif

#ifndef NO_PARALLEL_NETCODE
		Interfaces::MDLCache->BeginLock();
#endif
		if (!_didDecrypt2)
		{
			DecStr(leanyawstr2, 8);
			DecStr(speedstr2, 5);
			DecStr(ladderspeedstr2, 12);
			DecStr(ladderyawstr2, 10);
			DecStr(moveyawstr2, 8);
			DecStr(runstr2, 3);
			DecStr(bodyyawstr2, 8);
			DecStr(bodypitchstr2, 10);
			DecStr(deathyawstr2, 9);
			DecStr(standstr2, 5);
			DecStr(jumpfallstr2, 9);
			DecStr(aimblendstandidlestr2, 20);
			DecStr(aimblendcrouchidlestr2, 21);
			DecStr(str2afeyawstr2, 10);
			DecStr(aimblendstandwalkstr2, 20);
			DecStr(aimblendstandrunstr2, 19);
			DecStr(aimblendcrouchwalkstr2, 21);
			DecStr(moveblendwalkstr2, 15);
			DecStr(moveblendrunstr2, 14);
			DecStr(moveblendcrouchstr2, 17);
			_didDecrypt2 = true;
		}
#ifndef NO_PARALLEL_NETCODE
		Interfaces::MDLCache->EndLock();
#endif

		m_arrPoseParameters[0].Init(pBaseEntity, leanyawstr2);
		m_arrPoseParameters[1].Init(pBaseEntity, speedstr2);
		m_arrPoseParameters[2].Init(pBaseEntity, ladderspeedstr2);
		m_arrPoseParameters[3].Init(pBaseEntity, ladderyawstr2);
		m_arrPoseParameters[4].Init(pBaseEntity, moveyawstr2);
		if (m_iAnimsetVersion < 2)
			m_arrPoseParameters[5].Init(pBaseEntity, runstr2);
		m_arrPoseParameters[6].Init(pBaseEntity, bodyyawstr2);
		m_arrPoseParameters[7].Init(pBaseEntity, bodypitchstr2);
		m_arrPoseParameters[8].Init(pBaseEntity, deathyawstr2);
		m_arrPoseParameters[9].Init(pBaseEntity, standstr2);
		m_arrPoseParameters[10].Init(pBaseEntity, jumpfallstr2);
		m_arrPoseParameters[11].Init(pBaseEntity, aimblendstandidlestr2);
		m_arrPoseParameters[12].Init(pBaseEntity, aimblendcrouchidlestr2);
		m_arrPoseParameters[13].Init(pBaseEntity, str2afeyawstr2);
		m_arrPoseParameters[14].Init(pBaseEntity, aimblendstandwalkstr2);
		m_arrPoseParameters[15].Init(pBaseEntity, aimblendstandrunstr2);
		m_arrPoseParameters[16].Init(pBaseEntity, aimblendcrouchwalkstr2);
		if (m_iAnimsetVersion > 0)
		{
			m_arrPoseParameters[17].Init(pBaseEntity, moveblendwalkstr2);
			m_arrPoseParameters[18].Init(pBaseEntity, moveblendrunstr2);
			m_arrPoseParameters[19].Init(pBaseEntity, moveblendcrouchstr2);
		}

		m_nModelIndex = pBaseEntity->GetModelIndex();
	}
	return m_nModelIndex > 0;
}

//CLIENT
#pragma pack(push, 1)
struct C_CSGOPlayerAnimState2
{
	void* vtable;
	bool m_bIsReset;
	bool m_bUnknownClientBool;
	char pad[2];
	int m_iSomeTickcount;
	float m_flFlashedStartTime;
	float m_flFlashedEndTime;
	AimLayers m_AimLayers;
	int m_iUnknownClientArr[3];
	int m_nModelIndex;
	CBaseEntity* pBaseEntity;
	CBaseCombatWeapon* pActiveWeapon;
	CBaseCombatWeapon* pLastActiveWeapon;
	float m_flLastClientSideAnimationUpdateTime;
	int32_t m_iLastClientSideAnimationUpdateFramecount;
	float m_flLastClientSideAnimationUpdateTimeDelta;
	float m_flEyeYaw;
	float m_flPitch;
	float m_flGoalFeetYaw;
	float m_flCurrentFeetYaw;
	float m_flCurrentMoveDirGoalFeetDelta;
	float m_flGoalMoveDirGoalFeetDelta;
	float m_flFeetVelDirDelta;
	float pad_0094;
	float m_flFeetCycle;
	float m_flFeetWeight;
	float m_fUnknown2;
	float m_fDuckAmount;
	float m_flHitGroundCycle;
	float m_fUnknown3;
	Vector m_vOrigin;
	Vector m_vLastOrigin;
	Vector m_vVelocity;
	Vector m_vVelocityNormalized;
	Vector m_vecLastAcceleratingVelocity;
	float m_flSpeed;
	float m_flAbsVelocityZ;
	float m_flSpeedNormalized;
	float m_flRunningSpeed;
	float m_flDuckingSpeed;
	float m_flTimeSinceStartedMoving;
	float m_flTimeSinceStoppedMoving;
	bool m_bOnGround;
	bool m_bInHitGroundAnimation;
	char pad_010A[2];
	float m_flNextLowerBodyYawUpdateTime;
	float m_flTotalTimeInAir;
	float m_flStartJumpZOrigin;
	float m_flHitGroundWeight;
	float m_flGroundFraction;
	bool m_bJust_Landed;
	bool m_bJust_LeftGround;
	char pad_0120[2];
	float m_flDuckRate;
	bool m_bOnLadder;
	char pad_0128[3];
	float m_flLadderCycle;
	float m_flLadderWeight;
	bool m_bNotRunning;
	char pad_0135[3];
	bool m_bInBalanceAdjust;
	char pad_0141[3];
	CUtlVector<CUtlSymbol> m_ActivityModifiers;
	int gap148[1];
	float m_flTimeOfLastInjury;
	float m_flLastSetupLeanCurtime;
	Vector m_vecLastSetupLeanVelocity;
	Vector m_vecSetupLeanVelocityDelta;
	Vector m_vecSetupLeanVelocityInterpolated;
	float m_flLeanWeight;
	char pad_0184[8];
	bool m_bFlashed;
	char m_bFlashedPad[3];
	float m_flStrafeWeight;
	int m_iUnknownint3;
	float m_flStrafeCycle;
	int m_iStrafeSequence;
	bool m_bStrafing;
	char m_bStrafingPad[3];
	float m_flTotalStrafeTime;
	int m_iUnknownInt4;
	bool m_bUnknownBool__;
	bool m_bIsAccelerating;
	char pad_01AE[2];
	animstate_pose_param_cache_t m_arrPoseParameters[20];
	int m_iUnknownClientInt__;
	float m_flGoalRunningSpeed;
	int m_iMoveState;
	float m_flMovePlaybackRate;
	float m_flUnknownFL0;
	float m_flUnknownFL;
	float m_flUnknownFL1;
	float m_flMinYawServer;
	float m_flMaxYawServer;
	float m_flMaximumPitchServer;
	float m_flMinimumPitchServer;
	int m_iUnknownInt;
	char pad_02D0[84];
	float m_flEyePosZ;
	bool m_bIsDucked;
	char pad_0329[3];
	float m_flUnknownCap1;
	float m_flMinYaw;
	float m_flMaxYaw;
	float m_flMinPitch;
	float m_flMaxPitch;
	int m_iAnimsetVersion;


	C_AnimationLayer* GetAnimOverlay(int layer);
	void IncrementLayerCycle(int layer, bool dont_clamp_cycle);
	void IncrementLayerCycleGeneric(int layer);
	void LayerWeightAdvance(int layer);
	void SetFeetCycle(float cycle);
	int GetLayerActivity(int layer);
	void Update(float yaw, float pitch);
	void SetupFlinch();
	void SetupAliveloop();
	void SetupFlashedReaction();
	void SetupWholeBodyAction();
	void SetupMovement();
	void SetupLean();
	void SetupWeaponAction();
	void SetupAimMatrix();
	void SetupVelocity();
	bool CacheSequences();
	void SetLayerCycle(int layer, float cycle);
	void SetLayerCycleDirect(int layer, float cycle);
	void SetLayerWeight(int layer, float weight);
	void SetLayerSequence(int layer, int sequence);
	float GetLayerCycle(int layer);
	float GetLayerWeight(int layer);
	char* GetWeaponPrefix();
	void UpdateAnimLayer(float playbackrate, int layer, int sequence, float weight, float cycle);
	void UpdateLayerOrderPreset(float weight, int layer, int sequence);
	float GetLayerIdealWeightFromSeqCycle(int layer);
	void ModifyEyePosition(Vector& vecEyePos);
	void UpdateAimLayer(AimLayer *layer, float timedelta, float multiplier, bool somebool);
};
#pragma pack(pop)


/*
bool animstate_pose_param_cache_t::Init(CBaseEntity* pPlayer, char* name)
{
	ANIMATION_MUTEX_LOCK;
	m_szPoseParameter = name;
	int pose_index = pPlayer->LookupPoseParameter(pPlayer->GetModelPtr(), name);
	m_iPoseParameter = pose_index;
	if (pose_index != -1)
		m_bInitialized = true;
	ANIMATION_MUTEX_UNLOCK
	return m_bInitialized;
}

void animstate_pose_param_cache_t::SetValue(CBaseEntity* pPlayer, float flValue)
{
	bool initialized = m_bInitialized;

	if (!m_bInitialized)
		initialized = Init(pPlayer, m_szPoseParameter);

	if (initialized && pPlayer)
	{
		ANIMATION_MUTEX_LOCK;
		pPlayer->SetPoseParameterGame(pPlayer->GetModelPtr(), flValue, m_iPoseParameter);
		ANIMATION_MUTEX_UNLOCK
	}
}


float animstate_pose_param_cache_t::GetValue(CBaseEntity *pPlayer)
{
	if (!m_bInitialized)
		Init(pPlayer, m_szPoseParameter);

	if (m_bInitialized && pPlayer)
		return pPlayer->GetPoseParameter(m_iPoseParameter);

	return 0.0f;
}
*/

void CCSGOPlayerAnimState::ModifyEyePosition(Vector& vecEyePos)
{
	if (pBaseEntity && (m_bInHitGroundAnimation || m_fDuckAmount != 0.0f || !pBaseEntity->GetGroundEntity()))
	{
#ifndef NO_PARALLEL_NETCODE
		Interfaces::MDLCache->BeginLock();
#endif
		//decrypts(0)
		int bone = pBaseEntity->LookupBone((char*)XorStr("head_0"));
		//encrypts(0)
#ifndef NO_PARALLEL_NETCODE
		Interfaces::MDLCache->EndLock();
#endif
		if (bone != -1)
		{
			Vector vecHeadPos;
			QAngle vecHeadAngles;

			pBaseEntity->GetBonePosition(bone, vecHeadPos, vecHeadAngles);

			float flHeadHeight = vecHeadPos.z + 1.7f;
			if (flHeadHeight < vecEyePos.z)
			{
				float tmp = clamp((fabsf(vecEyePos.z - flHeadHeight) - 4.0f) * (1.0f / 6.0f), 0.0f, 1.0f);
				vecEyePos.z += ((flHeadHeight - vecEyePos.z) * (((powf(tmp, 2) * -2.0f) * tmp) + (3.0f * powf(tmp, 2))));
			}
		}
	}
}

void CCSGOPlayerAnimState::DoAnimationEvent(int event)
{
	UpdateActivityModifiers();
	switch (event)
	{
	case 0:
	case 7:
	{
		if (!pActiveWeapon || event || pActiveWeapon->GetItemDefinitionIndex() != WEAPON_C4)
		{
			int v5 = GetWeightedSequenceFromActivity(ACT_CSGO_FIRE_PRIMARY);
			SetLayerSequence(1, v5);
		}
		else
		{
			int v4 = GetWeightedSequenceFromActivity(ACT_CSGO_PLANT_BOMB);
			SetLayerSequence(8, v4);
			m_bPlantingBomb = 1;
		}
		break;
	}
	case 1:
	{
		int v6 = GetWeightedSequenceFromActivity(ACT_CSGO_FIRE_PRIMARY_OPT_1);
		SetLayerSequence(1, v6);
		return;
	}
	case 2:
	case 3:
	{
		int v7 = GetWeightedSequenceFromActivity(ACT_CSGO_FIRE_PRIMARY_OPT_2);
		SetLayerSequence(1, v7);
		return;
	}
	case 4:
	{
		if (pActiveWeapon && pActiveWeapon->GetWeaponType() == 5)
		{
			int v5 = GetWeightedSequenceFromActivity(ACT_CSGO_FIRE_PRIMARY);
			SetLayerSequence(1, v5);
		}
		else
		{
			int v9 = GetWeightedSequenceFromActivity(ACT_CSGO_FIRE_SECONDARY);
			SetLayerSequence(1, v9);
		}
		break;
	}
	case 5:
	{
		int v10 = GetWeightedSequenceFromActivity(ACT_CSGO_FIRE_SECONDARY_OPT_1);
		SetLayerSequence(1, v10);
		break;
	}
	case 6:
	{
		int v11 = GetWeightedSequenceFromActivity(ACT_CSGO_OPERATE);
		SetLayerSequence(1, v11);
		break;
	}
	case 8:
	{
		m_bJumping = 1;
		int v25 = GetWeightedSequenceFromActivity(ACT_CSGO_JUMP);
		SetLayerSequence(4, v25);
		break;
	}
	case 9:
	{
		if (!pActiveWeapon || pActiveWeapon->GetWeaponType() != 4 || pActiveWeapon->GetItemDefinitionIndex() == WEAPON_C4)
		{
			int v15 = GetWeightedSequenceFromActivity(ACT_CSGO_RELOAD);
			SetLayerSequence(1, v15);
		}
		break;
	}
	case 10:
	{
		int v16 = GetWeightedSequenceFromActivity(ACT_CSGO_RELOAD_START);
		SetLayerSequence(1, v16);
		break;
	}
	case 11:
	{
		int v17 = GetWeightedSequenceFromActivity(ACT_CSGO_RELOAD_LOOP);
		SetLayerSequence(1, v17);
		break;
	}
	case 12:
	{
		int v18 = GetWeightedSequenceFromActivity(ACT_CSGO_RELOAD_END);
		SetLayerSequence(1, v18);
		break;
	}
	case 13:
	{
		m_bPlantingBomb = 0;
		break;
	}
	case 14:
	{
		if (!pActiveWeapon || GetLayerActivity(1) != ACT_CSGO_DEPLOY)
		{
			int v24 = GetWeightedSequenceFromActivity(ACT_CSGO_DEPLOY);
			SetLayerSequence(1, v24);
		}
		else if (GetLayerCycle(1) >= 0.15f)
		{
			int v24 = GetWeightedSequenceFromActivity(ACT_CSGO_DEPLOY);
			SetLayerSequence(1, v24);
		}
		else
		{
			m_bDeploying = 1;
		}
		break;
	}
	case 15:
	{
		int v12 = GetWeightedSequenceFromActivity(ACT_CSGO_SILENCER_ATTACH);
		SetLayerSequence(1, v12);
		break;
	}
	case 16:
	{
		int v13 = GetWeightedSequenceFromActivity(ACT_CSGO_SILENCER_DETACH);
		SetLayerSequence(1, v13);
		break;
	}
	case 17:
	{
#ifndef NO_PARALLEL_NETCODE
		Interfaces::MDLCache->BeginLock();
#endif
		//decrypts(0)
		AddActivityModifier(XorStr("underhand"));
		//encrypts(0)
#ifndef NO_PARALLEL_NETCODE
		Interfaces::MDLCache->EndLock();
#endif
		if (!pActiveWeapon || event || pActiveWeapon->GetItemDefinitionIndex() != WEAPON_C4)
		{
			int v5 = GetWeightedSequenceFromActivity(ACT_CSGO_FIRE_PRIMARY);
			SetLayerSequence(1, v5);
		}
		else
		{
			int v4 = GetWeightedSequenceFromActivity(ACT_CSGO_PLANT_BOMB);
			SetLayerSequence(8, v4);
			m_bPlantingBomb = 1;
		}
		break;
	}
	case 18:
	{
		int v19 = GetWeightedSequenceFromActivity(ACT_CSGO_CATCH);
		SetLayerSequence(1, v19);
		break;
	}
	default:
		return;
	}
}

void CCSGOPlayerAnimState::IncrementLayerCycle(int layer, bool is_looping)
{
	C_AnimationLayer* pLayer = pBaseEntity->GetAnimOverlay(layer);
	if (!pLayer || fabs(pLayer->_m_flPlaybackRate) <= 0.0f)
		return;

	float newcycle = (pLayer->_m_flPlaybackRate * m_flLastClientSideAnimationUpdateTimeDelta) + pLayer->_m_flCycle;

	if (!is_looping && newcycle >= 1.0f)
		newcycle = 0.999f;

	newcycle -= (float)(int)newcycle; //round to integer

	if (newcycle < 0.0f)
		newcycle += 1.0f;

	if (newcycle > 1.0f)
		newcycle -= 1.0f;

	pLayer->SetCycle(newcycle);
}

void CCSGOPlayerAnimState::SetFeetCycle(float cycle)
{
	cycle -= (float)(int)cycle; //round to integer

	if (cycle < 0.0f)
		cycle += 1.0f;

	if (cycle > 1.0f)
		cycle -= 1.0f;

	m_flFeetCycle = cycle;
}

void  CCSGOPlayerAnimState::SetLayerPlaybackRate(int layer, float playbackrate)
{
	C_AnimationLayer* pLayer = pBaseEntity->GetAnimOverlay(layer);
	if (!pLayer)
		return;

	if (pLayer->_m_flPlaybackRate != playbackrate)
		pLayer->_m_flPlaybackRate = playbackrate;
}

void CCSGOPlayerAnimState::SetLayerCycle(int layer, float cycle)
{
	C_AnimationLayer* pLayer = pBaseEntity->GetAnimOverlay(layer);
	if (!pLayer)
		return;

	cycle -= (float)(int)cycle; //round to integer

	if (cycle < 0.0f)
		cycle += 1.0f;

	if (cycle > 1.0f)
		cycle -= 1.0f;

	pLayer->SetCycle(cycle);
}

void CCSGOPlayerAnimState::SetLayerCycleDirect(int layer, float cycle)
{
	C_AnimationLayer* pLayer = pBaseEntity->GetAnimOverlay(layer);
	if (!pLayer)
		return;

	pLayer->SetCycle(cycle);
}

void CCSGOPlayerAnimState::SetLayerWeight(int layer, float weight)
{
	C_AnimationLayer* pLayer = pBaseEntity->GetAnimOverlay(layer);
	if (!pLayer)
		return;

	weight = clamp(weight, 0.0f, 1.0f);

	pLayer->SetWeight(weight);
}

void CCSGOPlayerAnimState::SetLayerWeightDeltaRate(int layer, float oldweight)
{
	if (m_flLastClientSideAnimationUpdateTimeDelta != 0.0f)
	{
		C_AnimationLayer *pLayer = pBaseEntity->GetAnimOverlay(layer);
		float weightdeltarate = (pLayer->m_flWeight - oldweight) / m_flLastClientSideAnimationUpdateTimeDelta;
		if (pLayer->m_flWeightDeltaRate != weightdeltarate)
			pLayer->m_flWeightDeltaRate = weightdeltarate;
	}
}

void CCSGOPlayerAnimState::SetLayerSequence(int layer, int sequence)
{
	if (sequence >= 2)
	{
		ANIMATION_MUTEX_LOCK;
		C_AnimationLayer* pLayer = pBaseEntity->GetAnimOverlay(layer);
		if (pLayer)
		{
			pLayer->SetSequence(sequence);
			pLayer->_m_flPlaybackRate = pBaseEntity->GetLayerSequenceCycleRate(pLayer, sequence);
			pLayer->SetCycle(0.0f);
			pLayer->SetWeight(0.0f);
			UpdateLayerOrderPreset(0.0f, layer, sequence);
		}
		ANIMATION_MUTEX_UNLOCK
	}
}

int CCSGOPlayerAnimState::GetWeightedSequenceFromActivity(int activity)
{
	ANIMATION_MUTEX_LOCK;
	int new_seq = pBaseEntity->SelectWeightedSequenceFromModifiers(activity, m_ActivityModifiers.Base(), m_ActivityModifiers.Count());
	ANIMATION_MUTEX_UNLOCK
	return new_seq;
}

void CCSGOPlayerAnimState::SetLayerSequenceFromActivity(int layer, int activity)
{
	SetLayerSequence(layer, GetWeightedSequenceFromActivity(activity));
}

float CCSGOPlayerAnimState::GetLayerCycle(int layer)
{
	C_AnimationLayer* pLayer = pBaseEntity->GetAnimOverlay(layer);
	if (!pLayer)
		return 0.0f;

	return pLayer->_m_flCycle;
}

float CCSGOPlayerAnimState::GetLayerWeight(int layer)
{
	C_AnimationLayer* pLayer = pBaseEntity->GetAnimOverlay(layer);
	if (!pLayer)
		return 0.0f;

	return pLayer->m_flWeight;
}

void CCSGOPlayerAnimState::IncrementLayerCycleGeneric(int layer)
{
	C_AnimationLayer* pLayer = pBaseEntity->GetAnimOverlay(layer);
	if (!pLayer || fabs(pLayer->_m_flPlaybackRate) <= 0.0f)
		return;

	float newcycle = (pLayer->_m_flPlaybackRate * m_flLastClientSideAnimationUpdateTimeDelta) + pLayer->_m_flCycle;

	newcycle -= newcycle;

	if (newcycle < 0.0f)
		newcycle += 1.0f;

	if (newcycle > 1.0f)
		newcycle -= 1.0f;

	pLayer->SetCycle(newcycle);
}

bool CCSGOPlayerAnimState::IsLayerSequenceFinished(int layer)
{
	C_AnimationLayer *pLayer = pBaseEntity->GetAnimOverlay(layer);
	if (pLayer)
		return (m_flLastClientSideAnimationUpdateTimeDelta * pLayer->_m_flPlaybackRate) + pLayer->_m_flCycle >= 1.0f;
	return false;
}

bool CCSGOPlayerAnimState::LayerSequenceHasActMod(int layer, char* sequence)
{
	C_AnimationLayer *pLayer = pBaseEntity->GetAnimOverlay(layer);
	if (layer)
	{
		CStudioHdr *modelptr = pBaseEntity->GetModelPtr();
		mstudioseqdesc_t* seqdesc;
		int seq = pLayer->_m_nSequence;
		if (modelptr->m_pVModel)
		{
			seqdesc = opSeqdesc((studiohdr_t*)modelptr, seq);
		}
		else
		{
			seqdesc = modelptr->_m_pStudioHdr->pLocalSeqdesc(seq);
		}

		int num_ActivityModifiers = seqdesc->numactivitymodifiers;
		if (num_ActivityModifiers <= 0)
			return false;

		for (int i = 0; i < num_ActivityModifiers; ++i)
		{
			mstudioactivitymodifier_t *modifier = seqdesc->pActivityModifier(i);
			if (!strcmp(modifier->pszName(), sequence))
				return true;
		}
	}
	return false;
}

int CCSGOPlayerAnimState::GetLayerSequence(int layer)
{
	C_AnimationLayer* pLayer = pBaseEntity->GetAnimOverlay(layer);
	if (pLayer)
	{
		return pLayer->_m_nSequence;
	}
	return -1;
}

void CCSGOPlayerAnimState::SetupWeaponAction()
{
	float flNewLayer2_Weight = 0.0f;
	bool bContinueLayer1 = true;
	if (pActiveWeapon && m_bDeploying)
	{
		if (GetLayerActivity(AIMSEQUENCE_LAYER1) == ACT_CSGO_DEPLOY)
		{
			//FIXME
			//pActiveWeapon->ShowWeaponWorldModel(0);
			if (GetLayerCycle(AIMSEQUENCE_LAYER1) >= 0.15f)
			{
				m_bDeploying = false;
				int seq = GetWeightedSequenceFromActivity(ACT_CSGO_DEPLOY);
				SetLayerSequence(AIMSEQUENCE_LAYER1, seq);
				SetLayerWeight(AIMSEQUENCE_LAYER1, 0.0f);
				bContinueLayer1 = false;
			}
		}
	}

#ifndef NO_PARALLEL_NETCODE
	Interfaces::MDLCache->BeginLock();
#endif
	//decrypts(0)
	if (m_iAnimsetVersion < 2)
	{
		if (m_fDuckAmount > 0.0f)
		{
			if (GetLayerWeight(AIMSEQUENCE_LAYER1) > 0.0f)
			{
				if (LayerSequenceHasActMod(AIMSEQUENCE_LAYER1, XorStr("crouch")))
				{
					flNewLayer2_Weight = 0.0f;
				}
				else
				{
					if (GetLayerSequence(AIMSEQUENCE_LAYER2) <= 0)
					{
						SetLayerSequence(AIMSEQUENCE_LAYER2, pBaseEntity->LookupSequence(XorStr("recrouch_generic")));
					}
					flNewLayer2_Weight = GetLayerWeight(AIMSEQUENCE_LAYER1) * m_fDuckAmount;
				}
			}
		}
	}
	else
	{
		if (GetLayerWeight(AIMSEQUENCE_LAYER1) <= 0.0f)
		{
			if (GetLayerWeight(AIMSEQUENCE_LAYER2) > 0.0f)
			{
				flNewLayer2_Weight = Approach(0.0f, GetLayerWeight(AIMSEQUENCE_LAYER2), m_flLastClientSideAnimationUpdateTimeDelta * 4.0f);
			}
		}
		else
		{
			if (GetLayerSequence(AIMSEQUENCE_LAYER2) <= 0)
			{
				SetLayerSequence(AIMSEQUENCE_LAYER2, pBaseEntity->LookupSequence(XorStr("recrouch_generic")));
			}

			if (LayerSequenceHasActMod(AIMSEQUENCE_LAYER1, XorStr("crouch")))
			{
				if (m_fDuckAmount < 1.0f)
					flNewLayer2_Weight = GetLayerWeight(AIMSEQUENCE_LAYER1) * (1.0f - m_fDuckAmount);
				else
					flNewLayer2_Weight = 0.0f;
			}
			else if (m_fDuckAmount > 0.0f)
			{
				flNewLayer2_Weight = GetLayerWeight(AIMSEQUENCE_LAYER1) * (1.0f - m_fDuckAmount);
			}
			else
			{
				flNewLayer2_Weight = 0.0f;
			}
		}
	}
	//encrypts(0)
#ifndef NO_PARALLEL_NETCODE
	Interfaces::MDLCache->EndLock();
#endif

	SetLayerWeight(AIMSEQUENCE_LAYER2, flNewLayer2_Weight);

	if (bContinueLayer1)
	{
		IncrementLayerCycle(AIMSEQUENCE_LAYER1, false);
		float oldlayer2_weight = GetLayerWeight(AIMSEQUENCE_LAYER2);
		SetLayerWeight(AIMSEQUENCE_LAYER1, GetLayerIdealWeightFromSeqCycle(AIMSEQUENCE_LAYER1));
		SetLayerWeightDeltaRate(AIMSEQUENCE_LAYER1, oldlayer2_weight);
	}

	C_AnimationLayer *layer1 = pBaseEntity->GetAnimOverlay(AIMSEQUENCE_LAYER1);
	if (layer1)
	{
		if (pActiveWeapon)
		{
			CBaseEntity* worldmodel = Interfaces::ClientEntList->GetBaseEntityFromHandle(pActiveWeapon->GetWorldModelHandle());
			if (worldmodel)
			{
				ANIMATION_MUTEX_LOCK;
				if (layer1->m_iPriority <= 0 || layer1->m_flWeight <= 0.0f)
				{
					//FIXME:
					//if (worldmodel->GetCycle() != 0.0f)
					//	DispatchAnimEvents(worldmodel);
					worldmodel->SetSequenceVMT(0);
					worldmodel->SetCycle(0.0f);
					worldmodel->SetPlaybackRate(0.0f);
				}
				else
				{
					worldmodel->SetSequenceVMT(layer1->m_iPriority);
					worldmodel->SetCycle(layer1->_m_flCycle);
					worldmodel->SetPlaybackRate(layer1->_m_flPlaybackRate);
					//FIXME:
					//DispatchAnimEvents(worldmodel);
				}
				ANIMATION_MUTEX_UNLOCK
			}
		}
	}
}

void CCSGOPlayerAnimState::UpdateAimLayer(AimLayer *array, float timedelta, float multiplier, bool somebool)
{
	float v4;
	bool v5;
	float v7;

	if (somebool)
	{
		v4 = array->m_flUnknown0 + timedelta;
		array->m_flTotalTime = 0.0;
		v5 = v4 < array->m_flUnknown1;
		array->m_flUnknown0 = v4;

		if (v5)
			return;

		array->m_flWeight = Approach(1.0f, array->m_flWeight, multiplier);

		return;
	}

	v7 = array->m_flTotalTime + timedelta;
	array->m_flUnknown0 = 0.0;
	v5 = v7 < array->m_flUnknown2;
	array->m_flTotalTime = v7;

	if (v5)
		return;

	array->m_flWeight = Approach(0.0f, array->m_flWeight, multiplier);
}

void CCSGOPlayerAnimState::SetupAimMatrix()
{
	ANIMATION_MUTEX_LOCK;
	if (m_fDuckAmount <= 0.0f || m_fDuckAmount >= 1.0f)
	{
		bool is_walking = pBaseEntity && pBaseEntity->GetIsWalking();
		bool is_scoped;
		float speedmultiplier;
		if (pBaseEntity && pBaseEntity->IsScoped())
		{
			speedmultiplier = 4.2f;
			is_scoped = true;
		}
		else
		{
			speedmultiplier = 0.8f;
			is_scoped = false;
		}

		speedmultiplier *= m_flLastClientSideAnimationUpdateTimeDelta;

		if (is_scoped)
		{
			m_AimLayers.layers[0].m_flTotalTime = m_AimLayers.layers[0].m_flUnknown2;
			m_AimLayers.layers[1].m_flTotalTime = m_AimLayers.layers[1].m_flUnknown2;
			m_AimLayers.layers[2].m_flTotalTime = m_AimLayers.layers[2].m_flUnknown2;
		}

		bool v10 = is_walking && !is_scoped && m_flRunningSpeed > 0.7f && m_flSpeedNormalized < 0.7f;
		UpdateAimLayer(m_AimLayers.layers, m_flLastClientSideAnimationUpdateTimeDelta, speedmultiplier, v10);

		bool v12 = !is_scoped && m_flSpeedNormalized >= 0.7;
		UpdateAimLayer(&m_AimLayers.layers[1], m_flLastClientSideAnimationUpdateTimeDelta, speedmultiplier, v12);

		bool v14 = !is_scoped && m_flDuckingSpeed >= 0.5;
		UpdateAimLayer(&m_AimLayers.layers[2], m_flLastClientSideAnimationUpdateTimeDelta, speedmultiplier, v14);
	}

	float pose_crouch_idle = 1.0f;
	float strafe_weight = 1.0f;
	float aimlayer0_weight = m_AimLayers.layers[0].m_flWeight;
	float aimlayer1_weight = m_AimLayers.layers[1].m_flWeight;
	float aimlayer2_weight = m_AimLayers.layers[2].m_flWeight;
	if (aimlayer0_weight >= 1.0f)
		pose_crouch_idle = 0.0f;
	if (aimlayer1_weight >= 1.0f)
	{
		pose_crouch_idle = 0.0f;
		aimlayer0_weight = 0.0;
	}
	if (aimlayer2_weight >= 1.0f)
	{
		strafe_weight = 0.0f;
	}
	if (m_fDuckAmount < 1.0f)
	{
		if (m_fDuckAmount <= 0.0f)
		{
			strafe_weight = 0.0f;
			aimlayer2_weight = 0.0f;
		}
	}
	else
	{
		pose_crouch_idle = 0.0f;
		aimlayer0_weight = 0.0f;
		aimlayer1_weight = 0.0f;
	}

	float duck_pose = 1.0f - m_fDuckAmount;
	float pose_strafe_yaw = m_fDuckAmount * strafe_weight;
	float pose_move_blend_walk = m_fDuckAmount * aimlayer2_weight;

	float pose_stand_run = duck_pose * aimlayer0_weight;
	float pose_crouch_walk = duck_pose * aimlayer1_weight;

	if (pose_strafe_yaw < 1.0f && pose_move_blend_walk < 1.0f && pose_stand_run < 1.0f && pose_crouch_walk < 1.0f)
	{
		pose_crouch_idle = 1.0f;
	}

	m_arrPoseParameters[11].SetValue(pBaseEntity, pose_crouch_idle); //aim_blend_crouch_idle
	m_arrPoseParameters[14].SetValue(pBaseEntity, pose_stand_run); //aim_blend_stand_run
	m_arrPoseParameters[15].SetValue(pBaseEntity, pose_crouch_walk); //aim_blend_crouch_walk
	m_arrPoseParameters[12].SetValue(pBaseEntity, pose_strafe_yaw); //strafe_yaw
	m_arrPoseParameters[16].SetValue(pBaseEntity, pose_move_blend_walk); //move_blend_walk

#ifndef NO_PARALLEL_NETCODE
	Interfaces::MDLCache->BeginLock();
#endif
	char *anim = GetWeaponPrefix();
	//decrypts(0)
	char animation[64];
	sprintf_s(animation, XorStr("%s_aim"), anim);
	//encrypts(0)
#ifndef NO_PARALLEL_NETCODE
	Interfaces::MDLCache->EndLock();
#endif

	int aim_sequence = pBaseEntity->LookupSequence(anim);
	C_AnimationLayer *layer0 = pBaseEntity->GetAnimOverlay(0);
	if (layer0)
	{
		if (pActiveWeapon)
		{
			CBaseEntity* worldmodel = Interfaces::ClientEntList->GetBaseEntityFromHandle(pActiveWeapon->GetWorldModelHandle());
			CBaseEntity* sourcemodel = pBaseEntity;
			int sourcesequence = aim_sequence;
			if (worldmodel && layer0->m_iPriority != -1)
			{
				sourcemodel = worldmodel;
				sourcesequence = layer0->m_iPriority;
			}

			if (sourcesequence > 0)
			{
				float yawmin_idle = sourcemodel->GetAnySequenceAnimTag(sourcesequence, ANIMTAG_AIMLIMIT_YAWMIN_IDLE, -58.0f);
				float yawmax_idle = sourcemodel->GetAnySequenceAnimTag(sourcesequence, ANIMTAG_AIMLIMIT_YAWMAX_IDLE, 58.0f);
				float yawmin_walk = sourcemodel->GetAnySequenceAnimTag(sourcesequence, ANIMTAG_AIMLIMIT_YAWMIN_WALK, yawmin_idle);
				float yawmax_walk = sourcemodel->GetAnySequenceAnimTag(sourcesequence, ANIMTAG_AIMLIMIT_YAWMAX_WALK, yawmax_idle);
				float yawmin_run = sourcemodel->GetAnySequenceAnimTag(sourcesequence, ANIMTAG_AIMLIMIT_YAWMIN_RUN, yawmin_walk);
				float yawmax_run = sourcemodel->GetAnySequenceAnimTag(sourcesequence, ANIMTAG_AIMLIMIT_YAWMAX_RUN, yawmax_walk);
				float yawmin_crouchidle = sourcemodel->GetAnySequenceAnimTag(sourcesequence, ANIMTAG_AIMLIMIT_YAWMIN_CROUCHIDLE, -58.0f);
				float yawmax_crouchidle = sourcemodel->GetAnySequenceAnimTag(sourcesequence, ANIMTAG_AIMLIMIT_YAWMAX_CROUCHIDLE, 58.0f);
				float yawmin_crouchwalk = sourcemodel->GetAnySequenceAnimTag(sourcesequence, ANIMTAG_AIMLIMIT_YAWMIN_CROUCHWALK, yawmin_crouchidle);
				float yawmax_crouchwalk = sourcemodel->GetAnySequenceAnimTag(sourcesequence, ANIMTAG_AIMLIMIT_YAWMAX_CROUCHWALK, yawmax_crouchidle);

				float pose_blend_stand_run = m_arrPoseParameters[14].GetValue(pBaseEntity); //aim_blend_stand_run
				float pose_blend_crouch_walk = m_arrPoseParameters[15].GetValue(pBaseEntity); //aim_blend_crouch_walk
				pose_move_blend_walk = m_arrPoseParameters[16].GetValue(pBaseEntity); //move_blend_walk

				if (!pBaseEntity)
				{
					pose_move_blend_walk = 0.0f;
				}

				float v62 = ((yawmax_crouchwalk - yawmax_crouchidle) * pose_move_blend_walk) + yawmax_crouchidle;
				float v63 = ((yawmin_run - (((yawmin_walk - yawmin_idle) * pose_blend_stand_run) + yawmin_idle)) * pose_blend_crouch_walk)
					+ (((yawmin_walk - yawmin_idle) * pose_blend_stand_run) + yawmin_idle);

				m_flMinYaw = (((((yawmin_crouchwalk - yawmin_crouchidle) * pose_move_blend_walk) + yawmin_crouchidle)
					- v63)
					* m_fDuckAmount)
					+ v63;

				m_flMaxYaw = ((v62
					- (((yawmax_run
						- (((yawmax_walk - yawmax_idle) * pose_blend_stand_run) + yawmax_idle))
						* pose_blend_crouch_walk)
						+ (((yawmax_walk - yawmax_idle) * pose_blend_stand_run) + yawmax_idle)))
					* m_fDuckAmount)
					+ (((yawmax_run
						- (((yawmax_walk - yawmax_idle) * pose_blend_stand_run) + yawmax_idle))
						* pose_blend_crouch_walk)
						+ (((yawmax_walk - yawmax_idle) * pose_blend_stand_run) + yawmax_idle));

				float pitchmin_idle = sourcemodel->GetAnySequenceAnimTag(sourcesequence, ANIMTAG_AIMLIMIT_PITCHMIN_IDLE, -90.0f);
				float pitchmax_idle = sourcemodel->GetAnySequenceAnimTag(sourcesequence, ANIMTAG_AIMLIMIT_PITCHMAX_IDLE, 90.0f);
				float pitchmin_walkrun = sourcemodel->GetAnySequenceAnimTag(sourcesequence, ANIMTAG_AIMLIMIT_PITCHMIN_WALKRUN, pitchmin_idle);
				float pitchmax_walkrun = sourcemodel->GetAnySequenceAnimTag(sourcesequence, ANIMTAG_AIMLIMIT_PITCHMAX_WALKRUN, pitchmax_idle);
				float pitchmin_crouch = sourcemodel->GetAnySequenceAnimTag(sourcesequence, ANIMTAG_AIMLIMIT_PITCHMIN_CROUCH, -90.0f);
				float pitchmax_crouch = sourcemodel->GetAnySequenceAnimTag(sourcesequence, ANIMTAG_AIMLIMIT_PITCHMAX_CROUCH, 90.0f);
				float pitchmin_crouchwalk = sourcemodel->GetAnySequenceAnimTag(sourcesequence, ANIMTAG_AIMLIMIT_PITCHMIN_CROUCHWALK, pitchmin_crouch);
				float pitchmax_crouchwalk = sourcemodel->GetAnySequenceAnimTag(sourcesequence, ANIMTAG_AIMLIMIT_PITCHMAX_CROUCHWALK, pitchmax_crouch);

				float v67 = (((((pitchmax_crouchwalk - pitchmax_crouch) * pose_move_blend_walk) + pitchmax_crouch)
					- (((pitchmax_walkrun - pitchmax_idle) * pose_blend_stand_run) + pitchmax_idle))
					* m_fDuckAmount)
					+ (((pitchmax_walkrun - pitchmax_idle) * pose_blend_stand_run) + pitchmax_idle);

				m_flMinPitch = (((((pitchmin_crouchwalk - pitchmin_crouch) * pose_move_blend_walk) + pitchmin_crouch)
					- (((pitchmin_walkrun - pitchmin_idle) * pose_blend_stand_run)
						+ pitchmin_idle))
					* m_fDuckAmount)
					+ (((pitchmin_walkrun - pitchmin_idle) * pose_blend_stand_run)
						+ pitchmin_idle);

				m_flMaxPitch = v67;
			}
		}
	}

	UpdateAnimLayer(0.0f, MAIN_IDLE_SEQUENCE_LAYER, aim_sequence, 1.0f, 0.0f);

	ANIMATION_MUTEX_UNLOCK
}

void CCSGOPlayerAnimState::SetupLean()
{
	float timedelta = /*Interfaces::Globals->curtime*/ m_flCurTime - m_flLastSetupLeanCurtime;
	if (timedelta > 0.025f)
	{
		if (timedelta >= 0.1f)
			timedelta = 0.1f;

		Vector vecVelocity = pBaseEntity->GetVelocity();
		m_flLastSetupLeanCurtime = /*Interfaces::Globals->curtime*/ m_flCurTime;
		m_vecSetupLeanVelocityDelta.x = (vecVelocity.x - m_vecLastSetupLeanVelocity.x) * (1.0f / timedelta);
		m_vecSetupLeanVelocityDelta.y = (vecVelocity.y - m_vecLastSetupLeanVelocity.y) * (1.0f / timedelta);
		m_vecSetupLeanVelocityDelta.z = 0.0f;
		m_vecLastSetupLeanVelocity = vecVelocity;
	}

	m_vecSetupLeanVelocityInterpolated = g_LagCompensation.GetSmoothedVelocity(m_flLastClientSideAnimationUpdateTimeDelta * 800.0f, m_vecSetupLeanVelocityDelta, m_vecSetupLeanVelocityInterpolated);

	Vector pseudoup = Vector(0.0f, 0.0f, 1.0f);
	QAngle angles;
	VectorAngles(m_vecSetupLeanVelocityInterpolated, pseudoup, angles);

	float speed = (m_vecSetupLeanVelocityInterpolated.Length() * (1.0f / _CS_PLAYER_SPEED_RUN)) * m_flSpeedNormalized;

	m_flLeanWeight = (1.0f - m_flLadderCycle) * clamp(speed, 0.0f, 1.0f);

	float flGoalFeetLeanDelta = AngleNormalize(m_flGoalFeetYaw - angles.y);

	m_arrPoseParameters[0].SetValue(pBaseEntity, flGoalFeetLeanDelta);

	C_AnimationLayer *layer12 = pBaseEntity->GetAnimOverlay(LEAN_LAYER);
	if (!layer12 || layer12->_m_nSequence <= 0)
	{
		Interfaces::MDLCache->BeginLock();
		//decrypts(0)
		int seq = pBaseEntity->LookupSequence(XorStr("lean"));
		//encrypts(0)
		SetLayerSequence(LEAN_LAYER, seq);
		Interfaces::MDLCache->EndLock();
	}

	SetLayerWeight(LEAN_LAYER, m_flLeanWeight);
}

#pragma pack(push, 1)
struct C_CSGOPlayerAnimState_Small
{
	void* vtable;
	bool m_bIsReset;
	bool m_bUnknownClientBool;
	char pad[2];
	int m_iSomeTickcount;
	float m_flFlashedStartTime;
	float m_flFlashedEndTime;
	AimLayers m_AimLayers;
	int m_nModelIndex;
	int m_iUnknownClientArr[3];
	CBaseEntity* pBaseEntity;
	CBaseCombatWeapon* pActiveWeapon;
};
#pragma pack(pop)

char* CCSGOPlayerAnimState::GetWeaponPrefix()
{
	//do this until we rebuild that function
	C_CSGOPlayerAnimState_Small client_animstate_small;
	client_animstate_small.pBaseEntity = pBaseEntity;
	char* name = ((char*(__thiscall*)(CCSGOPlayerAnimState*))StaticOffsets.GetOffsetValue(_GetWeaponMoveAnimation))((CCSGOPlayerAnimState*)&client_animstate_small);
	pActiveWeapon = client_animstate_small.pActiveWeapon;
	return name;
}

/*
bool IsPlayingOldDemo()
{
	return (Interfaces::EngineClient->IsHLTV() || Interfaces::EngineClient->IsPlayingDemo()) && Interfaces::EngineClient->GetEngineBuildNumber() <= 13546;
}
*/

//custom function dylan added for brute force calculations
void CCSGOPlayerAnimState::UpdateSetupVelocityAndSetupLean(float yaw, float pitch)
{
	if (!pBaseEntity || (!pBaseEntity->GetAliveVMT() && !pBaseEntity->IsPlayerGhost()) || !CacheSequences())
		return;

	pitch = AngleNormalize(pBaseEntity->GetThirdPersonRecoil() + pitch);

	//if (m_flLastClientSideAnimationUpdateTime == /*Interfaces::Globals->curtime*/ m_flCurTime
	//	|| m_iLastClientSideAnimationUpdateFramecount == Interfaces::Globals->framecount)
	//	return;

	*s_bEnableInvalidateBoneCache = false; //not on server

	m_flLastClientSideAnimationUpdateTimeDelta = fmaxf(/*Interfaces::Globals->curtime*/ m_flCurTime - m_flLastClientSideAnimationUpdateTime, 0.0f);
	m_flEyeYaw = AngleNormalize(yaw);
	m_flPitch = AngleNormalize(pitch);
	m_vOrigin = *pBaseEntity->GetAbsOrigin();
	pActiveWeapon = pBaseEntity->GetActiveCSWeapon();
	if (pActiveWeapon != pLastActiveWeapon || m_bIsReset)
	{
		for (int i = 0; i < 13; ++i)
		{
			C_AnimationLayer *layer = pBaseEntity->GetAnimOverlay(i);
			if (layer)
			{
				layer->_m_fFlags = 0;
				layer->m_iActivity = -1;
				layer->m_iPriority = -1;
			}
		}
	}

	float flNewDuckAmount;

	if (!IsPlayingOldDemo())
	{
		flNewDuckAmount = clamp(pBaseEntity->GetDuckAmount() + m_flHitGroundCycle, 0.0f, 1.0f);
		flNewDuckAmount = Approach(flNewDuckAmount, m_fDuckAmount, m_flLastClientSideAnimationUpdateTimeDelta * 6.0f);
		flNewDuckAmount = clamp(flNewDuckAmount, 0.0f, 1.0f);
	}
	else
	{
		float flLandingAdjustment;
		if (pBaseEntity->GetFlags() & FL_ANIMDUCKING)
			flLandingAdjustment = 1.0f;
		else
			flLandingAdjustment = m_flHitGroundCycle;

		float flLandTimeAdjust;
		if (flLandingAdjustment <= m_fDuckAmount)
			flLandTimeAdjust = 6.0f;
		else
			flLandTimeAdjust = 3.1f;

		float flNewTimeDelta = m_flLastClientSideAnimationUpdateTimeDelta * flLandTimeAdjust;

		flNewDuckAmount = Approach(flLandingAdjustment, m_fDuckAmount, m_flLastClientSideAnimationUpdateTimeDelta * flLandTimeAdjust);
		m_fDuckAmount = flNewDuckAmount;

		flNewDuckAmount = clamp(flNewDuckAmount, 0.0f, 1.0f);
	}

	m_fDuckAmount = flNewDuckAmount;

	SetupVelocity();
	SetupAimMatrix();
	SetupMovement();

	for (int i = 0; i < 13; ++i)
	{
		C_AnimationLayer *layer = pBaseEntity->GetAnimOverlay(i);
		if (layer && !layer->_m_nSequence && layer->m_pOwner && layer->m_flWeight != 0.0f) //SetWeight inlined
		{
			layer->m_pOwner->InvalidatePhysicsRecursive(BOUNDS_CHANGED);
			layer->m_flWeight = 0.0f;
		}
	}

	pBaseEntity->SetAbsAngles(QAngle(0.0f, m_flGoalFeetYaw, 0.0f));
	pLastActiveWeapon = pActiveWeapon;
	m_vLastOrigin = m_vOrigin;
	m_bIsReset = 0;
	m_flLastClientSideAnimationUpdateTime = /*Interfaces::Globals->curtime*/ m_flCurTime;
	m_iLastClientSideAnimationUpdateFramecount = Interfaces::Globals->framecount;

	//NOT ON SERVER:
	*s_bEnableInvalidateBoneCache = true; //NOTE: NOT ON SERVER CODE
}

void CCSGOPlayerAnimState::Update(float yaw, float pitch)
{
	if (!pBaseEntity || (!pBaseEntity->GetAliveVMT() && !pBaseEntity->IsPlayerGhost()) || !CacheSequences())
		return;

	pitch = AngleNormalize(pBaseEntity->GetThirdPersonRecoil() + pitch);

	//if (m_flLastClientSideAnimationUpdateTime == /*Interfaces::Globals->curtime*/ m_flCurTime
	//	|| m_iLastClientSideAnimationUpdateFramecount == Interfaces::Globals->framecount)
	//	return;

	*s_bEnableInvalidateBoneCache = false; //not on server

	m_flLastClientSideAnimationUpdateTimeDelta = fmaxf(/*Interfaces::Globals->curtime*/ m_flCurTime - m_flLastClientSideAnimationUpdateTime, 0.0f);
	m_flEyeYaw = AngleNormalize(yaw);
	m_flPitch = AngleNormalize(pitch);
	m_vOrigin = *pBaseEntity->GetAbsOrigin();
	pActiveWeapon = pBaseEntity->GetActiveCSWeapon();
	if (pActiveWeapon != pLastActiveWeapon || m_bIsReset)
	{
		for (int i = 0; i < 13; ++i)
		{
			C_AnimationLayer *layer = pBaseEntity->GetAnimOverlay(i);
			if (layer)
			{
				layer->_m_fFlags = 0;
				layer->m_iActivity = -1;
				layer->m_iPriority = -1;
			}
		}
	}

	float flNewDuckAmount;

	if (!IsPlayingOldDemo())
	{
		flNewDuckAmount = clamp(pBaseEntity->GetDuckAmount() + m_flHitGroundCycle, 0.0f, 1.0f);
		flNewDuckAmount = Approach(flNewDuckAmount, m_fDuckAmount, m_flLastClientSideAnimationUpdateTimeDelta * 6.0f);
		flNewDuckAmount = clamp(flNewDuckAmount, 0.0f, 1.0f);
	}
	else
	{
		float flLandingAdjustment;
		if (pBaseEntity->GetFlags() & FL_ANIMDUCKING)
			flLandingAdjustment = 1.0f;
		else
			flLandingAdjustment = m_flHitGroundCycle;

		float flLandTimeAdjust;
		if (flLandingAdjustment <= m_fDuckAmount)
			flLandTimeAdjust = 6.0f;
		else
			flLandTimeAdjust = 3.1f;

		float flNewTimeDelta = m_flLastClientSideAnimationUpdateTimeDelta * flLandTimeAdjust;

		flNewDuckAmount = Approach(flLandingAdjustment, m_fDuckAmount, m_flLastClientSideAnimationUpdateTimeDelta * flLandTimeAdjust);
		m_fDuckAmount = flNewDuckAmount;

		flNewDuckAmount = clamp(flNewDuckAmount, 0.0f, 1.0f);
	}

	m_fDuckAmount = flNewDuckAmount;

	ANIMATION_MUTEX_LOCK;
	pBaseEntity->SetSequenceVMT(0);
	pBaseEntity->SetPlaybackRate(0.0f);
	if (pBaseEntity->GetCycle() != 0.0f) //SetCycle inlined
	{
		pBaseEntity->SetCycle(0.0f);
		pBaseEntity->InvalidatePhysicsRecursive(ANIMATION_CHANGED);
	}
	ANIMATION_MUTEX_UNLOCK

	SetupVelocity();
	SetupAimMatrix();
	SetupWeaponAction();
	SetupMovement();
	SetupAliveloop();
	SetupWholeBodyAction();
	SetupFlashedReaction();
	SetupFlinch();
	SetupLean();

	for (int i = 0; i < 13; ++i)
	{
		C_AnimationLayer *layer = pBaseEntity->GetAnimOverlay(i);
		if (layer && !layer->_m_nSequence && layer->m_pOwner && layer->m_flWeight != 0.0f) //SetWeight inlined
		{
			layer->m_pOwner->InvalidatePhysicsRecursive(BOUNDS_CHANGED);
			layer->m_flWeight = 0.0f;
		}
	}

	pBaseEntity->SetAbsAngles(QAngle(0.0f, m_flGoalFeetYaw, 0.0f));
	pLastActiveWeapon = pActiveWeapon;
	m_vLastOrigin = m_vOrigin;
	m_bIsReset = 0;
	m_flLastClientSideAnimationUpdateTime = /*Interfaces::Globals->curtime*/ m_flCurTime;
	m_iLastClientSideAnimationUpdateFramecount = Interfaces::Globals->framecount;

	//NOT ON SERVER:
	*s_bEnableInvalidateBoneCache = true; //NOTE: NOT ON SERVER CODE
}

void CCSGOPlayerAnimState::SetupVelocity()
{
	ANIMATION_MUTEX_LOCK;

	Vector velocity = m_vVelocity;
	pBaseEntity->GetAbsVelocity(velocity);

	//this is a hack to fix shuffling animations due to rounding errors, not part of actual game code
	if (fabsf(velocity.x) < 0.001)
		velocity.x = 0.0f;
	if (fabsf(velocity.y) < 0.001)
		velocity.y = 0.0f;
	if (fabsf(velocity.z) < 0.001)
		velocity.z = 0.0f;

	float spd = velocity.LengthSqr();

	m_flAbsVelocityZ = velocity.z;
	velocity.z = 0.0f;

	float leanspd = m_vecLastSetupLeanVelocity.LengthSqr();

	m_bIsAccelerating = velocity.Length2DSqr() > leanspd;

	m_vVelocity = g_LagCompensation.GetSmoothedVelocity(m_flLastClientSideAnimationUpdateTimeDelta * 2000.0f, velocity, m_vVelocity);

	m_vVelocityNormalized = VectorNormalizeReturn(m_vVelocity);

	float speed = std::fmin(m_vVelocity.Length(), 260.0f);
	m_flSpeed = speed;

	if (speed > 0.0f)
		m_vecLastAcceleratingVelocity = m_vVelocityNormalized;

	CBaseCombatWeapon *weapon = pBaseEntity->GetActiveCSWeapon();
	pActiveWeapon = weapon;

	float flMaxMovementSpeed = 260.0f;
	if (weapon)
		flMaxMovementSpeed = std::fmax(weapon->GetMaxSpeed3(), 0.001f);

	m_flSpeedNormalized = clamp(m_flSpeed / flMaxMovementSpeed, 0.0f, 1.0f);

	m_flRunningSpeed = m_flSpeed / (flMaxMovementSpeed * 0.520f);
	m_flDuckingSpeed = m_flSpeed / (flMaxMovementSpeed * 0.340f);

	if (m_flRunningSpeed < 1.0f)
	{
		if (m_flRunningSpeed < 0.5f)
		{
			m_flGoalRunningSpeed = Approach(80.0f, m_flGoalRunningSpeed, m_flLastClientSideAnimationUpdateTimeDelta * 60.0f);
		}
	}
	else
	{
		m_flGoalRunningSpeed = m_flSpeed;
	}

	bool m_bJustStoppedMoving = false;
	bool m_bJustStartedMoving = false;
	if (m_flSpeed <= 0.0f)
	{
		m_flTimeSinceStartedMoving = 0.0f;
		m_bJustStoppedMoving = m_flTimeSinceStoppedMoving <= 0.0f;
		m_flTimeSinceStoppedMoving += m_flLastClientSideAnimationUpdateTimeDelta;
	}
	else
	{
		m_flTimeSinceStoppedMoving = 0.0f;
		m_bJustStartedMoving = m_flTimeSinceStartedMoving <= 0.0f;
		m_flTimeSinceStartedMoving = m_flLastClientSideAnimationUpdateTimeDelta + m_flTimeSinceStartedMoving;
	}

	if (!m_bInBalanceAdjust
		&& m_bJustStoppedMoving
		&& m_bOnGround
		&& !m_bOnLadder
		&& !m_bInHitGroundAnimation
		&& m_flMovePlaybackRate < 50.0f)
	{
		SetLayerSequence(LOWERBODY_LAYER, GetWeightedSequenceFromActivity(ACT_CSGO_IDLE_ADJUST_STOPPEDMOVING));
		m_bInBalanceAdjust = true;
	}

	if (GetLayerActivity(LOWERBODY_LAYER) == ACT_CSGO_IDLE_ADJUST_STOPPEDMOVING || GetLayerActivity(LOWERBODY_LAYER) == ACT_CSGO_IDLE_TURN_BALANCEADJUST)
	{
		if (m_bInBalanceAdjust && m_flDuckingSpeed <= 0.25f)
		{
			float layer3_weight = GetLayerWeight(LOWERBODY_LAYER);
			IncrementLayerCycle(LOWERBODY_LAYER, false);
			SetLayerWeight(LOWERBODY_LAYER, GetLayerIdealWeightFromSeqCycle(LOWERBODY_LAYER));
			SetLayerWeightDeltaRate(LOWERBODY_LAYER, layer3_weight);
			m_bInBalanceAdjust = !IsLayerSequenceFinished(LOWERBODY_LAYER);
		}
		else
		{
			m_bInBalanceAdjust = false;
			float oldlayer3_weight = GetLayerWeight(LOWERBODY_LAYER);
			float newlayer3_weight = Approach(0.0f, oldlayer3_weight, m_flLastClientSideAnimationUpdateTimeDelta * 5.0f);
			SetLayerWeight(LOWERBODY_LAYER, newlayer3_weight);
			SetLayerWeightDeltaRate(LOWERBODY_LAYER, oldlayer3_weight);
		}
	}

	m_flCurrentFeetYaw = m_flGoalFeetYaw;
	m_flGoalFeetYaw = clamp(m_flGoalFeetYaw, -360.0f, 360.0f);

	float eye_feet_delta = AngleDiff(m_flEyeYaw, m_flGoalFeetYaw);

	float flRunningSpeed = clamp(m_flRunningSpeed, 0.0f, 1.0f);

	float flYawModifier = (((m_flGroundFraction * -0.3f) - 0.2f) * flRunningSpeed) + 1.0f;

	if (m_fDuckAmount > 0.0f)
	{
		float flDuckingSpeed = clamp(m_flDuckingSpeed, 0.0f, 1.0f);
		flYawModifier = flYawModifier + ((m_fDuckAmount * flDuckingSpeed) * (0.5f - flYawModifier));
	}

	float flMaxYawModifier = flYawModifier * m_flMaxYaw;
	float flMinYawModifier = flYawModifier * m_flMinYaw;

	if (eye_feet_delta <= flMaxYawModifier) // AngleDiff(m_flGoalFeetYaw, m_flEyeYaw) <= 58
	{
		if (flMinYawModifier > eye_feet_delta) // -58 > AngleDiff(m_flGoalFeetYaw, m_flEyeYaw)
			m_flGoalFeetYaw = fabs(flMinYawModifier) + m_flEyeYaw; // m_flGoalFeetYaw = fabs(58) + m_flEyeYaw
	}
	else
	{
		m_flGoalFeetYaw = m_flEyeYaw - fabs(flMaxYawModifier);
	}

	NormalizeAngle(m_flGoalFeetYaw);

	if (m_flSpeed > 0.1f || fabs(m_flAbsVelocityZ) > 100.0f)
	{
		m_flGoalFeetYaw = ApproachAngle(
			m_flEyeYaw,
			m_flGoalFeetYaw,
			((m_flGroundFraction * 20.0f) + 30.0f)
			* m_flLastClientSideAnimationUpdateTimeDelta);

		m_flNextLowerBodyYawUpdateTime = Interfaces::Globals->m_flCurTime + 0.22f;

		if (pBaseEntity->GetLowerBodyYaw() != m_flEyeYaw)
			pBaseEntity->SetLowerBodyYaw(m_flEyeYaw);
	}
	else
	{
		m_flGoalFeetYaw = ApproachAngle(
			pBaseEntity->GetLowerBodyYaw(),
			m_flGoalFeetYaw,
			m_flLastClientSideAnimationUpdateTimeDelta * 100.0f);

		if (Interfaces::Globals->m_flCurTime > m_flNextLowerBodyYawUpdateTime)
		{
			float m_flEyeFeetYawDelta = AngleDiff(m_flGoalFeetYaw, m_flEyeYaw);
			if (fabsf(m_flEyeFeetYawDelta) > 35.0f)
			{
				m_flNextLowerBodyYawUpdateTime = Interfaces::Globals->m_flCurTime + 1.1f;
				if (pBaseEntity->GetLowerBodyYaw() != m_flEyeYaw)
					pBaseEntity->SetLowerBodyYaw(m_flEyeYaw);
			}
		}
	}

	if (m_flSpeed <= 1.0f
		&& m_bOnGround
		&& !m_bOnLadder
		&& !m_bInHitGroundAnimation
		&& m_flLastClientSideAnimationUpdateTimeDelta > 0.0f
		&& (fabsf(AngleDiff(m_flCurrentFeetYaw, m_flGoalFeetYaw))
			/ m_flLastClientSideAnimationUpdateTimeDelta) > 120.0f)
	{
		SetLayerSequence(LOWERBODY_LAYER, GetWeightedSequenceFromActivity(ACT_CSGO_IDLE_TURN_BALANCEADJUST));
		m_bInBalanceAdjust = true;
	}

	C_AnimationLayer *layer3 = pBaseEntity->GetAnimOverlayDirect(3);
	if (layer3 && layer3->m_flWeight > 0.0f)
	{
		IncrementLayerCycle(3, false);
		LayerWeightAdvance(3);
	}

	if (m_flSpeed > 0.0f)
	{
		// turns speed into an angle
		float velAngle = (atan2(-m_vVelocity.y, -m_vVelocity.x) * 180.0f) * (1.0f / M_PI);

		if (velAngle < 0.0f)
			velAngle += 360.0f;

		m_flGoalMoveDirGoalFeetDelta = AngleNormalize(AngleDiff(velAngle, m_flGoalFeetYaw));
	}

	m_flFeetVelDirDelta = AngleNormalize(AngleDiff(m_flGoalMoveDirGoalFeetDelta, m_flCurrentMoveDirGoalFeetDelta));

	if (m_bJustStartedMoving && m_flFeetWeight <= 0.0f)
	{
		m_flCurrentMoveDirGoalFeetDelta = m_flGoalMoveDirGoalFeetDelta;

		int sequence = GetLayerSequence(6);
		if (sequence != -1)
		{
			if (pBaseEntity->pSeqdesc(sequence)->num_animtags > 0)
			{
				int tag = ANIMTAG_UNINITIALIZED;

				if (std::fabs(AngleDiff(m_flCurrentMoveDirGoalFeetDelta, 180.0f)) > 22.5f)
				{
					if (std::fabs(AngleDiff(m_flCurrentMoveDirGoalFeetDelta, 135.0f)) > 22.5f)
					{
						if (std::fabs(AngleDiff(m_flCurrentMoveDirGoalFeetDelta, 90.0f)) > 22.5f)
						{
							if (std::fabs(AngleDiff(m_flCurrentMoveDirGoalFeetDelta, 45.0f)) > 22.5f)
							{
								if (std::fabs(AngleDiff(m_flCurrentMoveDirGoalFeetDelta, 0.0f)) > 22.5f)
								{
									if (std::fabs(AngleDiff(m_flCurrentMoveDirGoalFeetDelta, -45.0f)) > 22.5f)
									{
										if (std::fabs(AngleDiff(m_flCurrentMoveDirGoalFeetDelta, -90.0f)) > 22.5f)
										{
											if (std::fabs(AngleDiff(m_flCurrentMoveDirGoalFeetDelta, -135.0f)) <= 22.5f)
												tag = ANIMTAG_STARTCYCLE_NW;
										}
										else
										{
											tag = ANIMTAG_STARTCYCLE_W;
										}
									}
									else
									{
										tag = ANIMTAG_STARTCYCLE_SW;
									}
								}
								else
								{
									tag = ANIMTAG_STARTCYCLE_S;
								}
							}
							else
							{
								tag = ANIMTAG_STARTCYCLE_SE;
							}
						}
						else
						{
							tag = ANIMTAG_STARTCYCLE_E;
						}
					}
					else
					{
						tag = ANIMTAG_STARTCYCLE_NE;
					}
				}
				else
				{
					tag = ANIMTAG_STARTCYCLE_N;
				}
				m_flFeetCycle = pBaseEntity->GetFirstSequenceAnimTag(sequence, tag);
			}
		}
	}
	else
	{
		if (GetLayerWeight(7) >= 1.0f)
		{
			m_flCurrentMoveDirGoalFeetDelta = m_flGoalMoveDirGoalFeetDelta;
		}
		else
		{
			float flDuckSpeedClamp = clamp(m_flDuckingSpeed, 0.0f, 1.0f);
			float flRunningSpeedClamp = clamp(m_flRunningSpeed, 0.0f, 1.0f);
			float flLerped = ((flDuckSpeedClamp - flRunningSpeedClamp) * m_fDuckAmount) + flRunningSpeedClamp;
			float flBiasMove = Bias(flLerped, 0.18f);
			m_flCurrentMoveDirGoalFeetDelta = AngleNormalize(((flBiasMove + 0.1f) * m_flFeetVelDirDelta) + m_flCurrentMoveDirGoalFeetDelta);
		}
	}

	m_arrPoseParameters[4].SetValue(pBaseEntity, m_flCurrentMoveDirGoalFeetDelta); //move_yaw

	float eye_goalfeet_delta = AngleDiff(m_flEyeYaw, m_flGoalFeetYaw);

	float new_body_yaw_pose = 0.0f; // not initialized?

	if (eye_goalfeet_delta < 0.0f || m_flMaxYaw == 0.0f)
	{
		if (m_flMinYaw != 0.0f)
			new_body_yaw_pose = (eye_goalfeet_delta / m_flMinYaw) * -58.0f;
	}
	else
	{
		new_body_yaw_pose = (eye_goalfeet_delta / m_flMaxYaw) * 58.0f;
	}

	m_arrPoseParameters[6].SetValue(pBaseEntity, new_body_yaw_pose);

	float eye_pitch_normalized = AngleNormalize(m_flPitch);
	float new_body_pitch_pose;

	if (eye_pitch_normalized <= 0.0f)
		new_body_pitch_pose = (eye_pitch_normalized / m_flMinPitch) * -90.0f;
	else
		new_body_pitch_pose = (eye_pitch_normalized / m_flMaxPitch) * 90.0f;

	m_arrPoseParameters[7].SetValue(pBaseEntity, new_body_pitch_pose);

	//Set pose parameter speed to m_flRunningSpeed
	m_arrPoseParameters[1].SetValue(pBaseEntity, m_flRunningSpeed);

	//Set pose parameter stand
	m_arrPoseParameters[9].SetValue(pBaseEntity, 1.0f - (m_flDuckRate * m_fDuckAmount));

	ANIMATION_MUTEX_UNLOCK
}

void CCSGOPlayerAnimState::SetupMovement()
{
	ANIMATION_MUTEX_LOCK;

	float old_ground_fraction = m_flGroundFraction;
	if (old_ground_fraction > 0.0f && old_ground_fraction < 1.0f)
	{
		float twotickstime = m_flLastClientSideAnimationUpdateTimeDelta + m_flLastClientSideAnimationUpdateTimeDelta;
		if (m_bNotRunning)
			m_flGroundFraction = old_ground_fraction - twotickstime;
		else
			m_flGroundFraction = twotickstime + old_ground_fraction;

		m_flGroundFraction = clamp(m_flGroundFraction, 0.0f, 1.0f);
	}

	if (m_flSpeed > (260.0f * _CS_PLAYER_SPEED_WALK_MODIFIER) && m_bNotRunning)
	{
		m_bNotRunning = false;
		m_flGroundFraction = fmaxf(m_flGroundFraction, 0.01f);
	}
	else if (m_flSpeed < (260.0f * _CS_PLAYER_SPEED_WALK_MODIFIER) && !m_bNotRunning)
	{
		m_bNotRunning = true;
		m_flGroundFraction = fminf(m_flGroundFraction, 0.99f);
	}

	if (m_iAnimsetVersion < 2)
	{
		m_arrPoseParameters[5].SetValue(pBaseEntity, m_flGroundFraction);
	}
	else
	{
		m_arrPoseParameters[17].SetValue(pBaseEntity, (1.0f - m_flGroundFraction) * (1.0f - m_fDuckAmount));
		m_arrPoseParameters[18].SetValue(pBaseEntity, (1.0f - m_fDuckAmount) * m_flGroundFraction);
		m_arrPoseParameters[19].SetValue(pBaseEntity, m_fDuckAmount);
	}

#ifndef NO_PARALLEL_NETCODE
	Interfaces::MDLCache->BeginLock();
#endif
	//decrypts(0)
	char dest[64];
	sprintf_s(dest, XorStr("move_%s"), GetWeaponPrefix());
	//encrypts(0)

	int seq = pBaseEntity->LookupSequence(dest);
	if (seq == -1)
	{
		//decrypts(0)
		seq = pBaseEntity->LookupSequence(XorStr("move"));
		//encrypts(0)
	}
#ifndef NO_PARALLEL_NETCODE
	Interfaces::MDLCache->EndLock();
#endif

	if (pBaseEntity->GetMoveState() != m_iMoveState)
		m_flMovePlaybackRate += 10.0f;

	m_iMoveState = pBaseEntity->GetMoveState();

	m_flMovePlaybackRate = clamp(Approach(0.0f, m_flMovePlaybackRate, m_flLastClientSideAnimationUpdateTimeDelta * 40.0f), 0.0f, 100.0f);

	float duckspeed_clamped = clamp(m_flDuckingSpeed, 0.0f, 1.0f);
	float runspeed_clamped = clamp(m_flRunningSpeed, 0.0f, 1.0f);

	float ideal_feet_weight = ((duckspeed_clamped - runspeed_clamped) * m_fDuckAmount) + runspeed_clamped;

	if (ideal_feet_weight < m_flFeetWeight)
	{
		float v34 = clamp(m_flMovePlaybackRate * 0.01f, 0.0f, 1.0f);
		float feetweight_speed_change = ((v34 * 18.0f) + 2.0f) * m_flLastClientSideAnimationUpdateTimeDelta;

		m_flFeetWeight = Approach(ideal_feet_weight, m_flFeetWeight, feetweight_speed_change);
	}
	else
	{
		m_flFeetWeight = ideal_feet_weight;
	}

	float yaw = AngleNormalize((m_flCurrentMoveDirGoalFeetDelta + m_flGoalFeetYaw) + 180.0f);
	QAngle angle = { 0.0f, yaw, 0.0f };
	Vector vecDir;
	AngleVectors(angle, &vecDir);

	float movement_side = DotProduct(m_vecLastAcceleratingVelocity, vecDir);
	if (movement_side < 0.0f)
		movement_side = -movement_side;

	float newfeetweight = Bias(movement_side, 0.2f) * m_flFeetWeight;

	m_flFeetWeight = newfeetweight;

	float newfeetweight2 = newfeetweight * m_flDuckRate;

	float layer5_weight = GetLayerWeight(LANDING_LAYER);

	float new_weight = 0.55f;
	if (1.0f - layer5_weight > 0.55f)
		new_weight = 1.0f - layer5_weight;

	float new_feet_layer_weight = new_weight * newfeetweight2;
	float feet_cycle_rate = 0.0f;

	if (m_flSpeed > 0.00f)
	{
		float seqcyclerate = pBaseEntity->GetSequenceCycleRate_Server(seq);

		float seqmovedist = pBaseEntity->GetSequenceMoveDist(pBaseEntity->GetModelPtr(), seq);
		seqmovedist *= 1.0f / (1.0f / seqcyclerate);
		if (seqmovedist <= 0.001f)
			seqmovedist = 0.001f;

		float speed_multiplier = m_flSpeed / seqmovedist;
		feet_cycle_rate = (1.0f - (m_flGroundFraction * 0.15f)) * (speed_multiplier * seqcyclerate);
	}

	float feetcycle_playback_rate = (m_flLastClientSideAnimationUpdateTimeDelta * feet_cycle_rate);
	SetFeetCycle(feetcycle_playback_rate + m_flFeetCycle);

	UpdateAnimLayer(feetcycle_playback_rate, FEET_LAYER, seq, clamp(new_feet_layer_weight, 0.0f, 1.0f), m_flFeetCycle);

	if (pBaseEntity->IsLocalPlayer()) 	//NOTICE: THIS CHECK DOES NOT EXIST IN THE GAME ITSELF, dylan added it so that the server anim code doesn't break strafing status
	{
		QAngle angle2 = { 0.0f, m_flGoalFeetYaw, 0.0f };
		Vector vecForward, vecRight, vecForwardVelocity;
		AngleVectors(angle2, &vecForward, &vecRight, nullptr);
		VectorNormalize(vecRight);
		vecForwardVelocity = m_vecLastAcceleratingVelocity * vecForward;

		float movement_side2 = DotProduct(m_vecLastAcceleratingVelocity, vecRight);
		int m_nButtons = pBaseEntity->GetButtons();

		bool v69 = false;
		float v70;
		if (m_nButtons & IN_MOVELEFT)
		{
			v70 = 1.0f;
		}
		else
		{
			v70 = 1.0f;
			if ((m_nButtons & IN_MOVERIGHT) && (m_flRunningSpeed >= 0.73f))
				v69 = movement_side2 < -0.63f;
		}
		bool v71 = false;
		if (!(m_nButtons & IN_MOVERIGHT) && (m_nButtons & IN_MOVELEFT) != 0 && m_flRunningSpeed >= 0.73f)
			v71 = movement_side2 > 0.63f;

		float flDiagonalVelocity = (vecForwardVelocity.x + vecForwardVelocity.y) + vecForwardVelocity.z;

		int bPressingForward = m_nButtons & IN_FORWARD;
		int bPressingBack = m_nButtons & IN_BACK;

		bool v74 = false;
		bool v76 = false;

		if (!bPressingBack && bPressingForward && (m_flRunningSpeed >= 0.65f))
			v74 = flDiagonalVelocity < -0.55f;

		if (!bPressingForward && bPressingBack && m_flRunningSpeed >= 0.65f)
			v76 = flDiagonalVelocity > 0.55f;

		bool bStrafing = true;

		if (!v71 && !v69)
			bStrafing = v76 || v74;

		if (pBaseEntity->IsStrafing() != bStrafing)
			pBaseEntity->SetIsStrafing(bStrafing);
	}

	if (pBaseEntity->IsStrafing())
	{
		if (!m_bStrafing)
		{
			m_flTotalStrafeTime = 0.0f;
		}

		m_bStrafing = true;

		m_flStrafeWeight = Approach(1.0f, m_flStrafeWeight, m_flLastClientSideAnimationUpdateTimeDelta * 20.0f);
		m_flStrafeCycle = Approach(0.0f, m_flStrafeCycle, m_flLastClientSideAnimationUpdateTimeDelta * 10.0f);

		float CurrentMoveDirGoalFeetDelta = AngleNormalize(m_flCurrentMoveDirGoalFeetDelta);

		m_arrPoseParameters[13].SetValue(pBaseEntity, CurrentMoveDirGoalFeetDelta);
	}
	else
	{
		if (m_arrPoseParameters[13].m_bInitialized)
		{
			if (m_flStrafeWeight > 0.0f)
			{
				m_flTotalStrafeTime += m_flLastClientSideAnimationUpdateTimeDelta;
				if (m_flTotalStrafeTime > 0.08f)
				{
					m_flStrafeWeight = Approach(0.0f, m_flStrafeWeight, m_flLastClientSideAnimationUpdateTimeDelta * 5.0f);
				}
#ifndef NO_PARALLEL_NETCODE
				Interfaces::MDLCache->BeginLock();
#endif
				//decrypts(0)
				m_iStrafeSequence = pBaseEntity->LookupSequence(XorStr("strafe"));
				//encrypts(0)
#ifndef NO_PARALLEL_NETCODE
				Interfaces::MDLCache->EndLock();
#endif
				m_flStrafeCycle += (m_flLastClientSideAnimationUpdateTimeDelta * pBaseEntity->GetSequenceCycleRate_Server(m_iStrafeSequence));
				m_flStrafeCycle = clamp(m_flStrafeCycle, 0.0f, 1.0f);
			}
		}
	}

	if (m_flStrafeWeight <= 0.0f)
		m_bStrafing = false;

	bool old_onground = m_bOnGround;
	bool new_onground = pBaseEntity->GetFlags() & FL_ONGROUND || pBaseEntity->GetWaterLevel() >= WL_Feet;
	m_bOnGround = new_onground;

	bool just_landed;
	if (!m_bIsReset && old_onground != new_onground && new_onground)
		just_landed = true;
	else
		just_landed = false;

	m_bJust_Landed = just_landed;

	bool just_left_ground = old_onground != new_onground && !new_onground;

	m_bJust_LeftGround = just_left_ground;

	if (just_left_ground)
		m_flStartJumpZOrigin = m_vOrigin.z;

	if (just_landed)
	{
		float zdelta = fabsf(m_flStartJumpZOrigin - m_vOrigin.z);
		zdelta = (zdelta - 12.0f) * 0.017f;
		zdelta = clamp(zdelta, 0.0f, 1.0f);

		float v91 = Bias(zdelta, 0.4f);
		float v94 = clamp(Bias(m_flTotalTimeInAir, 0.3f), 0.1f, 1.0f);

		m_flHitGroundWeight = v94;

		m_flHitGroundCycle = v94 <= v91 ? v91 : v94;
	}
	else
	{
		m_flHitGroundCycle = Approach(0.0f, m_flHitGroundCycle, m_flLastClientSideAnimationUpdateTimeDelta + m_flLastClientSideAnimationUpdateTimeDelta);
	}

	m_flDuckRate = Approach((float)(new_onground != 0), m_flDuckRate, ((m_fDuckAmount + 1.0f) * 8.0f) * m_flLastClientSideAnimationUpdateTimeDelta);
	m_flDuckRate = clamp(m_flDuckRate, 0.0f, 1.0f);

	float new_strafe_weight = ((1.0f - m_fDuckAmount) * m_flStrafeWeight) * m_flDuckRate;
	m_flStrafeWeight = clamp(new_strafe_weight, 0.0f, 1.0f);
	int strafe_sequence = m_iStrafeSequence;
	if (strafe_sequence != -1)
		UpdateAnimLayer(0.0f, SILENCERCHANGESEQUENCE_LAYER, strafe_sequence, m_flStrafeWeight, m_flStrafeCycle);

	bool old_onladder = m_bOnLadder;
	bool new_onladder = !m_bOnGround && pBaseEntity->GetMoveType() == MOVETYPE_LADDER;
	m_bOnLadder = new_onladder;

	bool just_got_on_ladder = false;
	bool not_onladder = false;

	if (!old_onladder && new_onladder)
	{
		just_got_on_ladder = true;
		not_onladder = false;
	}
	else
	{
		if (old_onladder)
		{
			not_onladder = true;
			if (new_onladder)
				not_onladder = false;
		}
	}

	if (m_flLadderCycle <= 0.0f && !new_onladder)
	{
		m_flLadderWeight = 0.0f;
	}
	else
	{
		if (just_got_on_ladder)
		{
			SetLayerSequenceFromActivity(LANDING_LAYER, ACT_CSGO_CLIMB_LADDER);
		}

		float laddertimedelta = m_flLastClientSideAnimationUpdateTimeDelta * 10.0f;
		float flNewLadderWeight;
		if (fabsf(m_flAbsVelocityZ) <= 100.0f)
		{
			flNewLadderWeight = Approach(0.0f, m_flLadderWeight, laddertimedelta);
		}
		else
		{
			flNewLadderWeight = Approach(1.0f, m_flLadderWeight, laddertimedelta);
		}

		m_flLadderWeight = clamp(flNewLadderWeight, 0.0f, 1.0f);

		float flNewLadderCycle;
		if (new_onladder)
		{
			flNewLadderCycle = Approach(1.0f, m_flLadderCycle, m_flLastClientSideAnimationUpdateTimeDelta * 5.0f);
		}
		else
		{
			flNewLadderCycle = Approach(0.0f, m_flLadderCycle, laddertimedelta);
		}
		m_flLadderCycle = clamp(flNewLadderCycle, 0.0f, 1.0f);

		Vector laddernormal = pBaseEntity->GetVecLadderNormal();
		QAngle ladderAngles;
		VectorAngles(laddernormal, ladderAngles);

		float ladderGoalFeetYawDelta = AngleDiff(ladderAngles.y, m_flGoalFeetYaw);

		m_arrPoseParameters[3].SetValue(pBaseEntity, ladderGoalFeetYawDelta);

		float layer5_cycle = GetLayerCycle(5);

		float unk = m_flLadderWeight * 0.006f;
		float newlayer5_cycle = ((m_vOrigin.z - m_vLastOrigin.z) * (0.01f - unk)) + layer5_cycle;
		m_arrPoseParameters[2].SetValue(pBaseEntity, m_flLadderWeight);

		if (GetLayerActivity(LANDING_LAYER) == ACT_CSGO_CLIMB_LADDER)
			SetLayerWeight(LANDING_LAYER, m_flLadderCycle);

		SetLayerCycle(LANDING_LAYER, newlayer5_cycle);

		if (m_bOnLadder)
		{
			float layer4_idealweight = 1.0f - m_flLadderCycle;

			if (GetLayerWeight(JUMPING_LAYER) > layer4_idealweight)
				SetLayerWeight(JUMPING_LAYER, layer4_idealweight);
		}
	}

	if (m_bOnGround)
	{
		if (!m_bInHitGroundAnimation)
		{
			if (m_bJust_Landed || not_onladder)
			{
				//Start the hit ground animation
				int landing_activity = m_flTotalTimeInAir > 1.0f ? ACT_CSGO_LAND_HEAVY : ACT_CSGO_LAND_LIGHT;
				int new_landing_sequence = GetWeightedSequenceFromActivity(landing_activity);
				SetLayerSequence(LANDING_LAYER, new_landing_sequence);
				SetLayerCycle(LANDING_LAYER, 0.0f);
				m_bInHitGroundAnimation = true;
			}
		}

		if (m_bInHitGroundAnimation && GetLayerActivity(5) != ACT_CSGO_CLIMB_LADDER)
		{
			m_bJumping = false;
			IncrementLayerCycle(LANDING_LAYER, false);
			IncrementLayerCycle(JUMPING_LAYER, false);
			m_arrPoseParameters[10].SetValue(pBaseEntity, 0.0f);

			if (IsLayerSequenceFinished(LANDING_LAYER))
			{
				//FINISHED HITTING GROUND ANIMATION
				m_bInHitGroundAnimation = false;
				SetLayerWeight(LANDING_LAYER, 0.0f);
				SetLayerWeight(JUMPING_LAYER, 0.0f);
				m_flHitGroundWeight = 1.0f;
			}
			else
			{
				//STILL IN HITTING GROUND ANIMATION

				float layer5_idealweight = GetLayerIdealWeightFromSeqCycle(LANDING_LAYER) * m_flHitGroundWeight;
				float weight_mult = 0.2f;
				float v168 = 1.0f - m_fDuckAmount;
				if (v168 >= 0.2f)
					weight_mult = fminf(v168, 1.0f);

				SetLayerWeight(LANDING_LAYER, weight_mult * layer5_idealweight);

				float layer4_weight = GetLayerWeight(JUMPING_LAYER);
				if (layer4_weight <= 0.0f)
				{
					ANIMATION_MUTEX_UNLOCK
					return;
				}

				SetLayerWeight(JUMPING_LAYER, Approach(0.0f, layer4_weight, m_flLastClientSideAnimationUpdateTimeDelta * 10.0f));
			}
		}

		if (!m_bInHitGroundAnimation && !m_bJumping && m_flLadderCycle <= 0.0f)
		{
			SetLayerWeight(LANDING_LAYER, 0.0f);
		}
	}
	else if (!m_bOnLadder)
	{
		//PLAYER IS IN THE AIR

		m_bInHitGroundAnimation = false;
		if (m_bJust_LeftGround || not_onladder)
		{
			if (!m_bJumping)
			{
				SetLayerSequenceFromActivity(JUMPING_LAYER, ACT_CSGO_FALL);
			}
			m_flTotalTimeInAir = 0.0f;
		}

		if (!m_bInParachute)
		{
			//FIXME
			if (/*pBaseEntity->GetParachuteState_NotNetworked() == 4 ||*/ pBaseEntity->IsSpawnRappelling())
			{
				int new_seq = GetWeightedSequenceFromActivity(ACT_CSGO_PARACHUTE);
				SetLayerSequence(JUMPING_LAYER, new_seq);
			}
		}

		//FIXME
		m_bInParachute = /*pBaseEntity->GetParachuteState_NotNetworked() == 4 ||*/ pBaseEntity->IsSpawnRappelling();
		m_flTotalTimeInAir += m_flLastClientSideAnimationUpdateTimeDelta;
		IncrementLayerCycle(JUMPING_LAYER, false);

		float layer4_weight = GetLayerWeight(JUMPING_LAYER);
		float layer4_idealweight = GetLayerIdealWeightFromSeqCycle(JUMPING_LAYER);

		if (layer4_idealweight > layer4_weight)
			SetLayerWeight(JUMPING_LAYER, layer4_idealweight);

		C_AnimationLayer *layer5 = pBaseEntity->GetAnimOverlay(LANDING_LAYER);
		if (layer5)
		{
			if (layer5->m_flWeight > 0.0f)
			{
				float v175 = (m_flTotalTimeInAir - 0.2f) * -5.0f;
				v175 = clamp(v175, 0.0f, 1.0f);
				float newlayer5_weight = ((3.0f - (v175 + v175)) * (v175 * v175)) * layer5->m_flWeight;
				SetLayerWeight(LANDING_LAYER, newlayer5_weight);
			}
		}

		float v203 = (m_flTotalTimeInAir - 0.72f) * 1.25f;
		v203 = clamp(v203, 0.0f, 1.0f);
		float newpose10 = (3.0f - (v203 + v203)) * (v203 * v203);
		newpose10 = clamp(newpose10, 0.0f, 1.0f);
		m_arrPoseParameters[10].SetValue(pBaseEntity, newpose10);
	}

	ANIMATION_MUTEX_UNLOCK
}

void CCSGOPlayerAnimState::SetupFlinch()
{
	float flTimeOfLastInjury = pBaseEntity->GetTimeOfLastInjury();
	if (flTimeOfLastInjury > m_flTimeOfLastInjury)
	{
		m_flTimeOfLastInjury = flTimeOfLastInjury;
		bool flinch_finished = false;
		bool is_head_flinching = false;
		C_AnimationLayer* flinchlayer = pBaseEntity->GetAnimOverlay(FLINCHSEQUENCE_LAYER);
		if (IsLayerSequenceFinished(FLINCHSEQUENCE_LAYER) || GetLayerWeight(FLINCHSEQUENCE_LAYER) <= 0.0f)
		{
			flinch_finished = true;
		}
		else
		{
			if (GetLayerActivity(FLINCHSEQUENCE_LAYER) == ACT_CSGO_FLINCH_HEAD)
				is_head_flinching = true;
		}

		if (pBaseEntity->GetFireCount() & 8)
		{
			if (flinch_finished)
			{
				int new_sequence = GetWeightedSequenceFromActivity(ACT_CSGO_FLINCH_MOLOTOV);// 984
				SetLayerSequence(FLINCHSEQUENCE_LAYER, GetWeightedSequenceFromActivity(ACT_CSGO_FLINCH_MOLOTOV));// 984
				UpdateActivityModifiers();
			}
		}
		else if (flinch_finished || !is_head_flinching || pBaseEntity->GetLastHitgroup() == HITGROUP_HEAD)
		{
			bool already_added_left_modifier = false;
			bool already_added_right_modifier = false;

#ifndef NO_PARALLEL_NETCODE
			Interfaces::MDLCache->BeginLock();
#endif
			//decrypts(0)
			switch (pBaseEntity->GetRelativeDirectionOfLastInjury())
			{
			case 0:
			case 1:
				AddActivityModifier(XorStr("front"));
				break;
			case 2:
				AddActivityModifier(XorStr("back"));
				break;
			case 3:
				AddActivityModifier(XorStr("left"));
				already_added_left_modifier = true;
				break;
			case 4:
				AddActivityModifier(XorStr("right"));
				already_added_right_modifier = true;
				break;
			default:
				break;
			}
			//encrypts(0)

			//decrypts(0)
			switch (pBaseEntity->GetLastHitgroup())
			{
			case HITGROUP_GENERIC:
			case HITGROUP_STOMACH:
				AddActivityModifier(XorStr("gut"));
				break;
			case HITGROUP_HEAD:
				AddActivityModifier(XorStr("head"));
				break;
			case HITGROUP_CHEST:
			case HITGROUP_NECK:
				AddActivityModifier(XorStr("chest"));
				break;
			case HITGROUP_LEFTARM:
				if (!already_added_left_modifier)
					AddActivityModifier(XorStr("left"));
				AddActivityModifier(XorStr("arm"));
				break;
			case HITGROUP_RIGHTARM:
				if (already_added_right_modifier)
					AddActivityModifier(XorStr("arm"));
				else
				{
					AddActivityModifier(XorStr("right"));
					AddActivityModifier(XorStr("arm"));
				}
				break;
			case HITGROUP_LEFTLEG:
				if (!already_added_left_modifier)
					AddActivityModifier(XorStr("left"));
				AddActivityModifier(XorStr("leg"));
				break;
			case HITGROUP_RIGHTLEG:
				if (!already_added_right_modifier)
					AddActivityModifier(XorStr("right"));
				AddActivityModifier(XorStr("leg"));
				break;
			default:
				break;
			}
			//encrypts(0)
#ifndef NO_PARALLEL_NETCODE
			Interfaces::MDLCache->EndLock();
#endif

			SetLayerSequenceFromActivity(10, pBaseEntity->GetLastHitgroup() == HITGROUP_HEAD ? ACT_CSGO_FLINCH_HEAD : ACT_CSGO_FLINCH);
			UpdateActivityModifiers();
		}
	}

	C_AnimationLayer* flinchlayer = pBaseEntity->GetAnimOverlay(FLINCHSEQUENCE_LAYER);
	float flNewWeight = 0.0f;
	if (flinchlayer && flinchlayer->_m_nSequence > 0)
		flNewWeight = GetLayerIdealWeightFromSeqCycle(FLINCHSEQUENCE_LAYER);
	SetLayerWeight(FLINCHSEQUENCE_LAYER, flNewWeight);
	IncrementLayerCycle(FLINCHSEQUENCE_LAYER, false);
}

//Done
void CCSGOPlayerAnimState::SetupAliveloop()
{
	if (GetLayerActivity(IDLE_LAYER) == ACT_CSGO_ALIVE_LOOP)
	{
		if (pActiveWeapon && pActiveWeapon != pLastActiveWeapon)
		{
			float flCycle = GetLayerCycle(IDLE_LAYER);
			int seq = GetWeightedSequenceFromActivity(ACT_CSGO_ALIVE_LOOP);
			SetLayerSequence(IDLE_LAYER, seq);
			SetLayerCycle(IDLE_LAYER, flCycle);
			IncrementLayerCycle(IDLE_LAYER, true);
		}
		else
		{
			C_AnimationLayer *layer11 = GetAnimOverlay(IDLE_LAYER);
			if (layer11 && ((layer11->_m_flPlaybackRate * m_flLastClientSideAnimationUpdateTimeDelta) + layer11->_m_flCycle) >= 1.0f)
			{
				ANIMATION_MUTEX_LOCK;
				if (GetAnimOverlay(IDLE_LAYER))
				{
					float dur = pBaseEntity->SequenceDuration(layer11->_m_nSequence);
					dur = dur <= 0.0f ? 10.0f : 1.0f / dur;
					float newcycle = RandomFloat(0.8f, 1.1f) * dur;
					SetLayerCycleDirect(IDLE_LAYER, newcycle);
				}
				ANIMATION_MUTEX_UNLOCK
				IncrementLayerCycle(IDLE_LAYER, true);
			}
			else
			{
				float flWeight = clamp((m_flSpeedNormalized - 0.55f) * (1.0f / 0.35f), 0.0f, 1.0f);
				SetLayerWeight(IDLE_LAYER, 1.0f - flWeight);
				IncrementLayerCycle(IDLE_LAYER, true);
			}
		}
	}
	else
	{
		ANIMATION_MUTEX_LOCK;
		SetLayerSequenceFromActivity(IDLE_LAYER, ACT_CSGO_ALIVE_LOOP);
		SetLayerCycle(IDLE_LAYER, RandomFloat(0.0f, 1.0f));
		C_AnimationLayer *layer11 = GetAnimOverlay(IDLE_LAYER);
		if (layer11)
		{
			float dur = pBaseEntity->SequenceDuration(layer11->_m_nSequence);
			dur = dur <= 0.0f ? 10.0f : 1.0f / dur;
			float newcycle = RandomFloat(0.8f, 1.1f) * dur;
			SetLayerCycleDirect(IDLE_LAYER, newcycle);
		}
		ANIMATION_MUTEX_UNLOCK
		IncrementLayerCycle(IDLE_LAYER, true);
	}
}

void CCSGOPlayerAnimState::IncrementLayerCycleWeightRateGeneric(int layer)
{
	float flOldWeight = GetLayerWeight(layer);
	IncrementLayerCycle(layer, false);
	float flNewWeight = GetLayerIdealWeightFromSeqCycle(layer);
	SetLayerWeight(layer, flNewWeight);
	SetLayerWeightDeltaRate(layer, flOldWeight);
}

void CCSGOPlayerAnimState::SetupWholeBodyAction()
{
	if (pBaseEntity->GetTeam() == TEAM_CT && pBaseEntity->IsDefusing())
	{
		if (m_bDefusing)
		{
			IncrementLayerCycleWeightRateGeneric(WHOLEBODYACTION_LAYER);
		}
		else
		{
			int activity = pBaseEntity->HasDefuseKit() ? ACT_CSGO_DEFUSE_WITH_KIT : ACT_CSGO_DEFUSE;
			int new_sequence = GetWeightedSequenceFromActivity(activity);
			SetLayerSequence(WHOLEBODYACTION_LAYER, new_sequence);
			m_bDefusing = true;
		}
	}
	else if (GetLayerActivity(WHOLEBODYACTION_LAYER) == ACT_CSGO_DEFUSE || GetLayerActivity(WHOLEBODYACTION_LAYER) == ACT_CSGO_DEFUSE_WITH_KIT)
	{
		if (GetLayerWeight(WHOLEBODYACTION_LAYER) > 0.0f)
		{
			float layer8_oldweight = GetLayerWeight(WHOLEBODYACTION_LAYER);
			float layer8_newweight = Approach(0.0f, layer8_oldweight, m_flLastClientSideAnimationUpdateTimeDelta * 8.0f);
			SetLayerWeight(WHOLEBODYACTION_LAYER, layer8_newweight);
			SetLayerWeightDeltaRate(WHOLEBODYACTION_LAYER, layer8_oldweight);
		}
		m_bDefusing = false;
	}
	else if (GetLayerActivity(WHOLEBODYACTION_LAYER) == ACT_CSGO_PLANT_BOMB)
	{
		if (pActiveWeapon)
		{
			if (pActiveWeapon->GetItemDefinitionIndex() != WEAPON_C4)
				m_bPlantingBomb = false;
		}
		float layer8_weight = GetLayerWeight(WHOLEBODYACTION_LAYER);
		if (m_bPlantingBomb)
		{
			IncrementLayerCycle(WHOLEBODYACTION_LAYER, false);
			float flIdealWeight = GetLayerIdealWeightFromSeqCycle(WHOLEBODYACTION_LAYER);
			float flNewWeight = Approach(flIdealWeight, layer8_weight, m_flLastClientSideAnimationUpdateTimeDelta * 1.2f);
			SetLayerWeight(WHOLEBODYACTION_LAYER, flNewWeight);
			SetLayerWeightDeltaRate(WHOLEBODYACTION_LAYER, layer8_weight);
			m_bPlantingBomb = !IsLayerSequenceFinished(WHOLEBODYACTION_LAYER);
		}
		else
		{
			if (layer8_weight > 0.0f)
			{
				float flNewLayer8_Weight = Approach(0.0f, GetLayerWeight(WHOLEBODYACTION_LAYER), m_flLastClientSideAnimationUpdateTimeDelta * 12.0f);
				SetLayerWeight(WHOLEBODYACTION_LAYER, flNewLayer8_Weight);
				SetLayerWeightDeltaRate(WHOLEBODYACTION_LAYER, layer8_weight);
			}
			m_bPlantingBomb = false;
		}
	}
	else
	{
		SetLayerCycle(WHOLEBODYACTION_LAYER, 0.999f);
		SetLayerWeight(WHOLEBODYACTION_LAYER, 0.0f);
		SetLayerWeightDeltaRate(WHOLEBODYACTION_LAYER, 0.0f);
	}
}

void CCSGOPlayerAnimState::UpdateAnimLayer(float playbackrate, int layer, int sequence, float weight, float cycle)
{
	if (sequence >= 2)
	{
		ANIMATION_MUTEX_LOCK;
		C_AnimationLayer *pLayer = pBaseEntity->GetAnimOverlay(layer);
		if (pLayer)
		{
			pLayer->SetSequence(sequence);
			pLayer->_m_flPlaybackRate = playbackrate;
			pLayer->SetCycle(clamp(cycle, 0.0f, 1.0f));
			pLayer->SetWeight(clamp(weight, 0.0f, 1.0f));
			UpdateLayerOrderPreset(weight, layer, sequence);
		}
		ANIMATION_MUTEX_UNLOCK
	}
}

void CCSGOPlayerAnimState::UpdateLayerOrderPreset(float weight, int layer, int sequence)
{
	if (pBaseEntity && layer == 1)
	{
		ANIMATION_MUTEX_LOCK;
		if (pBaseEntity->GetAnySequenceAnimTag(sequence, ANIMTAG_FLASHBANG_PASSABLE, 0.0f) == 0.0f)
			ApplyLayerOrderPreset(s_animLayerOrderDefault, 0);
		else
			ApplyLayerOrderPreset(s_animLayerOrderWeaponPost, 0);
		ANIMATION_MUTEX_UNLOCK
	}
}

float CCSGOPlayerAnimState::GetLayerIdealWeightFromSeqCycle(int layer)
{
	ANIMATION_MUTEX_LOCK;
	C_AnimationLayer *overlay = pBaseEntity->GetAnimOverlay(layer);
	if (!overlay)
	{
		ANIMATION_MUTEX_UNLOCK
		return 0.0f;
	}

	int sequence = overlay->_m_nSequence;
	CStudioHdr *modelptr = pBaseEntity->GetModelPtr();
	mstudioseqdesc_t *seqdesc;
	//NOTE TO VALVE: you don't check for valid pointer here!
	if (modelptr->m_pVModel)
		seqdesc = opSeqdesc((studiohdr_t*)modelptr, sequence);
	else
		seqdesc = modelptr->_m_pStudioHdr->pLocalSeqdesc(sequence);

	float cycle = overlay->_m_flCycle;
	if (cycle >= 0.99f)
		cycle = 1.0f;

	float fadeintime = seqdesc->fadeintime;
	float fadeouttime = seqdesc->fadeouttime;
	float weight = 1.0f;
	float v15;

	if (fadeintime <= 0.0f || fadeintime <= cycle)
	{
		if (fadeouttime >= 1.0f || cycle <= fadeouttime)
		{
			weight = fminf(weight, 1.0f);
			ANIMATION_MUTEX_UNLOCK
			return weight;
		}
		v15 = (cycle - 1.0f) / (fadeouttime - 1.0f);
		v15 = clamp(v15, 0.0f, 1.0f);
	}
	else
	{
		v15 = clamp(cycle / fadeintime, 0.0f, 1.0f);
	}
	weight = (3.0f - (v15 + v15)) * (v15 * v15);
	if (weight < 0.0015f)
		weight = 0.0f;
	else
		weight = clamp(weight, 0.0f, 1.0f);

	ANIMATION_MUTEX_UNLOCK
	return weight;
}

void CCSGOPlayerAnimState::SetupFlashedReaction()
{
	if (/*Interfaces::Globals->curtime*/ m_flCurTime <= m_flFlashedEndTime)
	{
		if (!m_bFlashed)
		{
			SetLayerSequenceFromActivity(FLASHEDSEQUENCE_LAYER, ACT_CSGO_FLASHBANG_REACTION);
			m_bFlashed = true;
		}

		float fllNewLayer9Weight;

		if (m_flFlashedStartTime == m_flFlashedEndTime)
		{
			if ((/*Interfaces::Globals->curtime*/ m_flCurTime - m_flFlashedEndTime) < 0.0f)
				fllNewLayer9Weight = 1.0f;
			else
				fllNewLayer9Weight = 0.0f;
		}
		else
		{
			float flNewWeightTotal = clamp(
				(/*Interfaces::Globals->curtime*/ m_flCurTime - m_flFlashedStartTime) / (m_flFlashedEndTime - m_flFlashedStartTime),
				0.0f, 1.0f
				);
			fllNewLayer9Weight = 1.0f - flNewWeightTotal;
		}

		SetLayerCycle(FLASHEDSEQUENCE_LAYER, 0.0f);
		SetLayerPlaybackRate(FLASHEDSEQUENCE_LAYER, 0.0f);

		float oldlayer9_weight = GetLayerWeight(FLASHEDSEQUENCE_LAYER);

		float newlayer9_weight = ((fllNewLayer9Weight * fllNewLayer9Weight) * 3.0f) - (((fllNewLayer9Weight * fllNewLayer9Weight) * 2.0f) * fllNewLayer9Weight);
		SetLayerWeight(FLASHEDSEQUENCE_LAYER, newlayer9_weight);
		if (newlayer9_weight < oldlayer9_weight)
			SetLayerWeightDeltaRate(FLASHEDSEQUENCE_LAYER, oldlayer9_weight);
		else
			SetLayerWeightDeltaRate(FLASHEDSEQUENCE_LAYER, 0.0f);
	}
	else
	{
		SetLayerWeight(FLASHEDSEQUENCE_LAYER, 0.0f);
		m_bFlashed = false;
	}
}

void CCSGOPlayerAnimState::LayerWeightAdvance(int layer)
{
	C_AnimationLayer* pLayer = pBaseEntity->GetAnimOverlay(layer);
	if (!layer || fabs(pLayer->m_flWeightDeltaRate) <= 0.0f)
		return;

	float newweight = (pLayer->m_flWeightDeltaRate * m_flLastClientSideAnimationUpdateTimeDelta) + pLayer->m_flWeight;
	newweight = clamp(newweight, 0.0f, 1.0f);
	pLayer->SetWeight(newweight);
}

C_AnimationLayer* CCSGOPlayerAnimState::GetAnimOverlay(int layer)
{
	CUtlVectorSimple* m_AnimOverlay = pBaseEntity->GetAnimOverlayStruct();
	int numoverlays = m_AnimOverlay->count;
	int maxindex = numoverlays - 1;
	int index = layer;
	if (maxindex >= 0)
	{
		if (layer > maxindex)
			index = numoverlays - 1;
	}
	else
	{
		index = 0;
	}

	if (numoverlays)
		return (C_AnimationLayer*)m_AnimOverlay->Retrieve(index, sizeof(C_AnimationLayer));

	return nullptr;
}

int CCSGOPlayerAnimState::GetLayerActivity(int layer)
{
	C_AnimationLayer* pLayer = GetAnimOverlay(layer);
	if (!pLayer)
		return -1;

	return GetSequenceActivity(pBaseEntity, pLayer->_m_nSequence);
}
