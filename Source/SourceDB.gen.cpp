#include "C:\Users\Usuario\Downloads\gam300-the-second-option2\Source\Enemies\EnemyParticleEmitter.h"
#include "C:\Users\Usuario\Downloads\gam300-the-second-option2\Source\Enemies\EnemySpawner.h"
#include "C:\Users\Usuario\Downloads\gam300-the-second-option2\Source\Enemies\Mobs.h"
#include "C:\Users\Usuario\Downloads\gam300-the-second-option2\Source\Player\Camera\FreeFlightCamera.h"
#include "C:\Users\Usuario\Downloads\gam300-the-second-option2\Source\Player\Camera\PlayerCam.h"
#include "C:\Users\Usuario\Downloads\gam300-the-second-option2\Source\Player\DashImage.h"
#include "C:\Users\Usuario\Downloads\gam300-the-second-option2\Source\Player\Player.h"
#include "C:\Users\Usuario\Downloads\gam300-the-second-option2\Source\Player\PlayerLight.h"
#include "C:\Users\Usuario\Downloads\gam300-the-second-option2\Source\Player\PowerUps.h"
#include "C:\Users\Usuario\Downloads\gam300-the-second-option2\Source\Player\Projectile.h"
#include "C:\Users\Usuario\Downloads\gam300-the-second-option2\Source\Resources\Floating.h"
#include "C:\Users\Usuario\Downloads\gam300-the-second-option2\Source\Zone\ClickAButtonToContinue.h"
#include "C:\Users\Usuario\Downloads\gam300-the-second-option2\Source\Zone\Door.h"
#include "C:\Users\Usuario\Downloads\gam300-the-second-option2\Source\Zone\LevelBoundaries.h"
#include "C:\Users\Usuario\Downloads\gam300-the-second-option2\Source\Zone\Props.h"
#include "C:\Users\Usuario\Downloads\gam300-the-second-option2\Source\Zone\Teleport.h"
#include "C:\Users\Usuario\Downloads\gam300-the-second-option2\Source\Zone\Zone.h"
/*This file has been automatically generated, 
DO NOT MODIFY UNDER ANY CIRCUMSTANCE, 
//
Good Neighbours Header Tool - 2021*/

#include<Shared.h>
template<typename T>  class TCreator : public Engine::Factory::ICreator {Engine::IBase* Create() override {return new T();}};
int DllMain() {
	static bool loaded = false;
	if(!loaded) {

	Engine::Factory::Instance().RegisterCustomCreator<Gameplay ::EnemyParticleEmitter>("Gameplay ::EnemyParticleEmitter");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay ::EnemySpawner>("Gameplay ::EnemySpawner");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay ::Mobs>("Gameplay ::Mobs");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay ::ZombieMob>("Gameplay ::ZombieMob");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay ::TurretMob>("Gameplay ::TurretMob");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay ::DashImageComp>("Gameplay ::DashImageComp");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay::Player>("Gameplay::Player");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay::PlayerMeleeParentComp>("Gameplay::PlayerMeleeParentComp");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay::PlayerMeleeComp>("Gameplay::PlayerMeleeComp");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay::PlayerMeleeEnergy>("Gameplay::PlayerMeleeEnergy");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay::PlayerLight>("Gameplay::PlayerLight");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay ::PowerUpShop>("Gameplay ::PowerUpShop");
	Engine::Factory::Instance().RegisterCustomCreator<Projectile>("Projectile");
	Engine::Factory::Instance().RegisterCustomCreator<EnemyProjectile>("EnemyProjectile");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay ::FreeFlightCamera>("Gameplay ::FreeFlightCamera");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay ::FreeHUD>("Gameplay ::FreeHUD");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay ::GraphicsTextUpdater>("Gameplay ::GraphicsTextUpdater");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay ::LogicTextUpdater>("Gameplay ::LogicTextUpdater");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay ::PhysicsTextUpdater>("Gameplay ::PhysicsTextUpdater");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay ::MuteAudio>("Gameplay ::MuteAudio");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay ::GeneralRotation>("Gameplay ::GeneralRotation");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay ::PlayerCam>("Gameplay ::PlayerCam");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay ::FloatAnim>("Gameplay ::FloatAnim");
	Engine::Factory::Instance().RegisterCustomCreator<PressStartToContinue>("PressStartToContinue");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay ::Door>("Gameplay ::Door");
	Engine::Factory::Instance().RegisterCustomCreator<LevelBoundaries>("LevelBoundaries");
	Engine::Factory::Instance().RegisterCustomCreator<PropBlinker>("PropBlinker");
	Engine::Factory::Instance().RegisterCustomCreator<PropWires>("PropWires");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay ::Teleport>("Gameplay ::Teleport");
	Engine::Factory::Instance().RegisterCustomCreator<Gameplay ::Zone>("Gameplay ::Zone");	loaded = true;}


	return 1;

}

void Gameplay ::EnemyParticleEmitter::ToJson(Gameplay ::EnemyParticleEmitter::json& j) const {
}
void Gameplay ::EnemyParticleEmitter::FromJson(const Gameplay ::EnemyParticleEmitter::json& j) {
}
#ifdef _EDITOR_
void Gameplay ::EnemyParticleEmitter::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
}
#endif

void Gameplay ::EnemySpawner::ToJson(Gameplay ::EnemySpawner::json& j) const {
	j["mID"] = mID;
	j["mIsLinked"] = mIsLinked;
	j["mWaves"] = mWaves;
	j["mSpawnCount"] = mSpawnCount;
	j["mRange"] = mRange;
	j["mTimetoSpawn"] = mTimetoSpawn;
	j["mIgnoreTimeifAllDead"] = mIgnoreTimeifAllDead;
	j["mTimeFirstSpawn"] = mTimeFirstSpawn;
}
void Gameplay ::EnemySpawner::FromJson(const Gameplay ::EnemySpawner::json& j) {
	auto it_mID = j.find("mID");
	if(it_mID != j.end()) mID = (*it_mID).get<int>();
	auto it_mIsLinked = j.find("mIsLinked");
	if(it_mIsLinked != j.end()) mIsLinked = (*it_mIsLinked).get<bool>();
	auto it_mWaves = j.find("mWaves");
	if(it_mWaves != j.end()) mWaves = (*it_mWaves).get<int>();
	auto it_mSpawnCount = j.find("mSpawnCount");
	if(it_mSpawnCount != j.end()) mSpawnCount = (*it_mSpawnCount).get<int>();
	auto it_mRange = j.find("mRange");
	if(it_mRange != j.end()) mRange = (*it_mRange).get<float>();
	auto it_mTimetoSpawn = j.find("mTimetoSpawn");
	if(it_mTimetoSpawn != j.end()) mTimetoSpawn = (*it_mTimetoSpawn).get<float>();
	auto it_mIgnoreTimeifAllDead = j.find("mIgnoreTimeifAllDead");
	if(it_mIgnoreTimeifAllDead != j.end()) mIgnoreTimeifAllDead = (*it_mIgnoreTimeifAllDead).get<bool>();
	auto it_mTimeFirstSpawn = j.find("mTimeFirstSpawn");
	if(it_mTimeFirstSpawn != j.end()) mTimeFirstSpawn = (*it_mTimeFirstSpawn).get<float>();
}
#ifdef _EDITOR_
void Gameplay ::EnemySpawner::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"mID", &mID};
	_UIDraw << std::pair{"mIsLinked", &mIsLinked};
	_UIDraw << std::pair{"mWaves", &mWaves};
	_UIDraw << std::pair{"mSpawnCount", &mSpawnCount};
	_UIDraw << std::pair{"mRange", &mRange};
	_UIDraw << std::pair{"mTimetoSpawn", &mTimetoSpawn};
	_UIDraw << std::pair{"mIgnoreTimeifAllDead", &mIgnoreTimeifAllDead};
	_UIDraw << std::pair{"mTimeFirstSpawn", &mTimeFirstSpawn};
}
#endif

void Gameplay ::Mobs::ToJson(Gameplay ::Mobs::json& j) const {
	j["mID"] = mID;
	j["mCurrentHealth"] = mCurrentHealth;
	j["mMaxHealth"] = mMaxHealth;
	j["mIdleDelay"] = mIdleDelay;
	j["mSpeed"] = mSpeed;
	j["mDamage"] = mDamage;
	j["mRadius"] = mRadius;
	j["mMyID"] = mMyID;
}
void Gameplay ::Mobs::FromJson(const Gameplay ::Mobs::json& j) {
	auto it_mID = j.find("mID");
	if(it_mID != j.end()) mID = (*it_mID).get<int>();
	auto it_mCurrentHealth = j.find("mCurrentHealth");
	if(it_mCurrentHealth != j.end()) mCurrentHealth = (*it_mCurrentHealth).get<int>();
	auto it_mMaxHealth = j.find("mMaxHealth");
	if(it_mMaxHealth != j.end()) mMaxHealth = (*it_mMaxHealth).get<int>();
	auto it_mIdleDelay = j.find("mIdleDelay");
	if(it_mIdleDelay != j.end()) mIdleDelay = (*it_mIdleDelay).get<float>();
	auto it_mSpeed = j.find("mSpeed");
	if(it_mSpeed != j.end()) mSpeed = (*it_mSpeed).get<float>();
	auto it_mDamage = j.find("mDamage");
	if(it_mDamage != j.end()) mDamage = (*it_mDamage).get<int>();
	auto it_mRadius = j.find("mRadius");
	if(it_mRadius != j.end()) mRadius = (*it_mRadius).get<float>();
	auto it_mMyID = j.find("mMyID");
	if(it_mMyID != j.end()) mMyID = (*it_mMyID).get<int>();
}
#ifdef _EDITOR_
void Gameplay ::Mobs::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"mID", &mID};
	_UIDraw << std::pair{"mCurrentHealth", &mCurrentHealth};
	_UIDraw << std::pair{"mMaxHealth", &mMaxHealth};
	_UIDraw << std::pair{"mIdleDelay", &mIdleDelay};
	_UIDraw << std::pair{"mSpeed", &mSpeed};
	_UIDraw << std::pair{"mDamage", &mDamage};
	_UIDraw << std::pair{"mRadius", &mRadius};
	_UIDraw << std::pair{"mMyID", &mMyID};
}
#endif

void Gameplay ::ZombieMob::ToJson(Gameplay ::ZombieMob::json& j) const {
	j["mSpeed"] = mSpeed;
}
void Gameplay ::ZombieMob::FromJson(const Gameplay ::ZombieMob::json& j) {
	auto it_mSpeed = j.find("mSpeed");
	if(it_mSpeed != j.end()) mSpeed = (*it_mSpeed).get<float>();
}
#ifdef _EDITOR_
void Gameplay ::ZombieMob::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"mSpeed", &mSpeed};
}
#endif

void Gameplay ::TurretMob::ToJson(Gameplay ::TurretMob::json& j) const {
	j["mMyID"] = mMyID;
	j["mDelayTime"] = mDelayTime;
}
void Gameplay ::TurretMob::FromJson(const Gameplay ::TurretMob::json& j) {
	auto it_mMyID = j.find("mMyID");
	if(it_mMyID != j.end()) mMyID = (*it_mMyID).get<int>();
	auto it_mDelayTime = j.find("mDelayTime");
	if(it_mDelayTime != j.end()) mDelayTime = (*it_mDelayTime).get<float>();
}
#ifdef _EDITOR_
void Gameplay ::TurretMob::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"mMyID", &mMyID};
	_UIDraw << std::pair{"mDelayTime", &mDelayTime};
}
#endif

void Gameplay ::DashImageComp::ToJson(Gameplay ::DashImageComp::json& j) const {
	j["mAliveDuration"] = mAliveDuration;
	j["mAlphaSubstractor"] = mAlphaSubstractor;
	j["mMinimumAlpha"] = mMinimumAlpha;
	j["mMaximumAlpha"] = mMaximumAlpha;
}
void Gameplay ::DashImageComp::FromJson(const Gameplay ::DashImageComp::json& j) {
	auto it_mAliveDuration = j.find("mAliveDuration");
	if(it_mAliveDuration != j.end()) mAliveDuration = (*it_mAliveDuration).get<float>();
	auto it_mAlphaSubstractor = j.find("mAlphaSubstractor");
	if(it_mAlphaSubstractor != j.end()) mAlphaSubstractor = (*it_mAlphaSubstractor).get<float>();
	auto it_mMinimumAlpha = j.find("mMinimumAlpha");
	if(it_mMinimumAlpha != j.end()) mMinimumAlpha = (*it_mMinimumAlpha).get<float>();
	auto it_mMaximumAlpha = j.find("mMaximumAlpha");
	if(it_mMaximumAlpha != j.end()) mMaximumAlpha = (*it_mMaximumAlpha).get<float>();
}
#ifdef _EDITOR_
void Gameplay ::DashImageComp::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"mAliveDuration", &mAliveDuration};
	_UIDraw << std::pair{"mAlphaSubstractor", &mAlphaSubstractor};
	_UIDraw << std::pair{"mMinimumAlpha", &mMinimumAlpha};
	_UIDraw << std::pair{"mMaximumAlpha", &mMaximumAlpha};
}
#endif

void Gameplay::Player::ToJson(Gameplay::Player::json& j) const {
	j["mControllerNumber"] = mControllerNumber;
	j["mRotSpeed"] = mRotSpeed;
	j["mScore"] = mScore;
	j["mMaxHP"] = mMaxHP;
	j["mMaxLives"] = mMaxLives;
	j["mDebugMode"] = mDebugMode;
	j["mPushDuration"] = mPushDuration;
	j["mDashSpeed"] = mDashSpeed;
	j["mDashDuration"] = mDashDuration;
	j["mDashTotalCD"] = mDashTotalCD;
	j["mConsequentDashCount"] = mConsequentDashCount;
	j["mBaseSpeed"] = mBaseSpeed;
}
void Gameplay::Player::FromJson(const Gameplay::Player::json& j) {
	auto it_mControllerNumber = j.find("mControllerNumber");
	if(it_mControllerNumber != j.end()) mControllerNumber = (*it_mControllerNumber).get<unsigned>();
	auto it_mRotSpeed = j.find("mRotSpeed");
	if(it_mRotSpeed != j.end()) mRotSpeed = (*it_mRotSpeed).get<float>();
	auto it_mScore = j.find("mScore");
	if(it_mScore != j.end()) mScore = (*it_mScore).get<unsigned>();
	auto it_mMaxHP = j.find("mMaxHP");
	if(it_mMaxHP != j.end()) mMaxHP = (*it_mMaxHP).get<int>();
	auto it_mMaxLives = j.find("mMaxLives");
	if(it_mMaxLives != j.end()) mMaxLives = (*it_mMaxLives).get<int>();
	auto it_mDebugMode = j.find("mDebugMode");
	if(it_mDebugMode != j.end()) mDebugMode = (*it_mDebugMode).get<bool>();
	auto it_mPushDuration = j.find("mPushDuration");
	if(it_mPushDuration != j.end()) mPushDuration = (*it_mPushDuration).get<float>();
	auto it_mDashSpeed = j.find("mDashSpeed");
	if(it_mDashSpeed != j.end()) mDashSpeed = (*it_mDashSpeed).get<float>();
	auto it_mDashDuration = j.find("mDashDuration");
	if(it_mDashDuration != j.end()) mDashDuration = (*it_mDashDuration).get<float>();
	auto it_mDashTotalCD = j.find("mDashTotalCD");
	if(it_mDashTotalCD != j.end()) mDashTotalCD = (*it_mDashTotalCD).get<float>();
	auto it_mConsequentDashCount = j.find("mConsequentDashCount");
	if(it_mConsequentDashCount != j.end()) mConsequentDashCount = (*it_mConsequentDashCount).get<int>();
	auto it_mBaseSpeed = j.find("mBaseSpeed");
	if(it_mBaseSpeed != j.end()) mBaseSpeed = (*it_mBaseSpeed).get<int>();
}
#ifdef _EDITOR_
void Gameplay::Player::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"mControllerNumber", &mControllerNumber};
	_UIDraw << std::pair{"mRotSpeed", &mRotSpeed};
	_UIDraw << std::pair{"mScore", &mScore};
	_UIDraw << std::pair{"mMaxHP", &mMaxHP};
	_UIDraw << std::pair{"mMaxLives", &mMaxLives};
	_UIDraw << std::pair{"mDebugMode", &mDebugMode};
	_UIDraw << std::pair{"mPushDuration", &mPushDuration};
	_UIDraw << std::pair{"mDashSpeed", &mDashSpeed};
	_UIDraw << std::pair{"mDashDuration", &mDashDuration};
	_UIDraw << std::pair{"mDashTotalCD", &mDashTotalCD};
	_UIDraw << std::pair{"mConsequentDashCount", &mConsequentDashCount};
	_UIDraw << std::pair{"mBaseSpeed", &mBaseSpeed};
}
#endif

void Gameplay::PlayerMeleeParentComp::ToJson(Gameplay::PlayerMeleeParentComp::json& j) const {
}
void Gameplay::PlayerMeleeParentComp::FromJson(const Gameplay::PlayerMeleeParentComp::json& j) {
}
#ifdef _EDITOR_
void Gameplay::PlayerMeleeParentComp::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
}
#endif

void Gameplay::PlayerMeleeComp::ToJson(Gameplay::PlayerMeleeComp::json& j) const {
}
void Gameplay::PlayerMeleeComp::FromJson(const Gameplay::PlayerMeleeComp::json& j) {
}
#ifdef _EDITOR_
void Gameplay::PlayerMeleeComp::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
}
#endif

void Gameplay::PlayerMeleeEnergy::ToJson(Gameplay::PlayerMeleeEnergy::json& j) const {
}
void Gameplay::PlayerMeleeEnergy::FromJson(const Gameplay::PlayerMeleeEnergy::json& j) {
}
#ifdef _EDITOR_
void Gameplay::PlayerMeleeEnergy::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
}
#endif

void Gameplay::PlayerLight::ToJson(Gameplay::PlayerLight::json& j) const {
	j["distance"] = distance;
}
void Gameplay::PlayerLight::FromJson(const Gameplay::PlayerLight::json& j) {
	auto it_distance = j.find("distance");
	if(it_distance != j.end()) distance = (*it_distance).get<float>();
}
#ifdef _EDITOR_
void Gameplay::PlayerLight::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"distance", &distance};
}
#endif

void Gameplay ::PowerUpShop::ToJson(Gameplay ::PowerUpShop::json& j) const {
	j["powerup_type"] = powerup_type;
}
void Gameplay ::PowerUpShop::FromJson(const Gameplay ::PowerUpShop::json& j) {
	auto it_powerup_type = j.find("powerup_type");
	if(it_powerup_type != j.end()) powerup_type = (*it_powerup_type).get<int>();
}
#ifdef _EDITOR_
void Gameplay ::PowerUpShop::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"powerup_type", &powerup_type};
}
#endif

void Projectile::ToJson(Projectile::json& j) const {
	j["mSpeed"] = mSpeed;
	j["mDamage"] = mDamage;
	j["mLifetime"] = mLifetime;
}
void Projectile::FromJson(const Projectile::json& j) {
	auto it_mSpeed = j.find("mSpeed");
	if(it_mSpeed != j.end()) mSpeed = (*it_mSpeed).get<float>();
	auto it_mDamage = j.find("mDamage");
	if(it_mDamage != j.end()) mDamage = (*it_mDamage).get<int>();
	auto it_mLifetime = j.find("mLifetime");
	if(it_mLifetime != j.end()) mLifetime = (*it_mLifetime).get<float>();
}
#ifdef _EDITOR_
void Projectile::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"mSpeed", &mSpeed};
	_UIDraw << std::pair{"mDamage", &mDamage};
	_UIDraw << std::pair{"mLifetime", &mLifetime};
}
#endif

void EnemyProjectile::ToJson(EnemyProjectile::json& j) const {
	j["mSpeed"] = mSpeed;
	j["mDamage"] = mDamage;
	j["mLifetime"] = mLifetime;
}
void EnemyProjectile::FromJson(const EnemyProjectile::json& j) {
	auto it_mSpeed = j.find("mSpeed");
	if(it_mSpeed != j.end()) mSpeed = (*it_mSpeed).get<float>();
	auto it_mDamage = j.find("mDamage");
	if(it_mDamage != j.end()) mDamage = (*it_mDamage).get<float>();
	auto it_mLifetime = j.find("mLifetime");
	if(it_mLifetime != j.end()) mLifetime = (*it_mLifetime).get<float>();
}
#ifdef _EDITOR_
void EnemyProjectile::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"mSpeed", &mSpeed};
	_UIDraw << std::pair{"mDamage", &mDamage};
	_UIDraw << std::pair{"mLifetime", &mLifetime};
}
#endif

void Gameplay ::FreeFlightCamera::ToJson(Gameplay ::FreeFlightCamera::json& j) const {
	j["mSensitivity"] = mSensitivity;
}
void Gameplay ::FreeFlightCamera::FromJson(const Gameplay ::FreeFlightCamera::json& j) {
	auto it_mSensitivity = j.find("mSensitivity");
	if(it_mSensitivity != j.end()) mSensitivity = (*it_mSensitivity).get<float>();
}
#ifdef _EDITOR_
void Gameplay ::FreeFlightCamera::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"mSensitivity", &mSensitivity};
}
#endif

void Gameplay ::FreeHUD::ToJson(Gameplay ::FreeHUD::json& j) const {
	j["mSensitivity"] = mSensitivity;
}
void Gameplay ::FreeHUD::FromJson(const Gameplay ::FreeHUD::json& j) {
	auto it_mSensitivity = j.find("mSensitivity");
	if(it_mSensitivity != j.end()) mSensitivity = (*it_mSensitivity).get<float>();
}
#ifdef _EDITOR_
void Gameplay ::FreeHUD::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"mSensitivity", &mSensitivity};
}
#endif

void Gameplay ::GraphicsTextUpdater::ToJson(Gameplay ::GraphicsTextUpdater::json& j) const {
}
void Gameplay ::GraphicsTextUpdater::FromJson(const Gameplay ::GraphicsTextUpdater::json& j) {
}
#ifdef _EDITOR_
void Gameplay ::GraphicsTextUpdater::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
}
#endif

void Gameplay ::LogicTextUpdater::ToJson(Gameplay ::LogicTextUpdater::json& j) const {
}
void Gameplay ::LogicTextUpdater::FromJson(const Gameplay ::LogicTextUpdater::json& j) {
}
#ifdef _EDITOR_
void Gameplay ::LogicTextUpdater::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
}
#endif

void Gameplay ::PhysicsTextUpdater::ToJson(Gameplay ::PhysicsTextUpdater::json& j) const {
}
void Gameplay ::PhysicsTextUpdater::FromJson(const Gameplay ::PhysicsTextUpdater::json& j) {
}
#ifdef _EDITOR_
void Gameplay ::PhysicsTextUpdater::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
}
#endif

void Gameplay ::MuteAudio::ToJson(Gameplay ::MuteAudio::json& j) const {
}
void Gameplay ::MuteAudio::FromJson(const Gameplay ::MuteAudio::json& j) {
}
#ifdef _EDITOR_
void Gameplay ::MuteAudio::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
}
#endif

void Gameplay ::GeneralRotation::ToJson(Gameplay ::GeneralRotation::json& j) const {
	j["speed"] = speed;
}
void Gameplay ::GeneralRotation::FromJson(const Gameplay ::GeneralRotation::json& j) {
	auto it_speed = j.find("speed");
	if(it_speed != j.end()) speed = (*it_speed).get<float>();
}
#ifdef _EDITOR_
void Gameplay ::GeneralRotation::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"speed", &speed};
}
#endif

void Gameplay ::PlayerCam::ToJson(Gameplay ::PlayerCam::json& j) const {
	j["mCamHeight"] = mCamHeight;
	j["mCamDist_X"] = mCamDist_X;
	j["mCamDist_Z"] = mCamDist_Z;
	j["mPlayer1"] = mPlayer1;
}
void Gameplay ::PlayerCam::FromJson(const Gameplay ::PlayerCam::json& j) {
	auto it_mCamHeight = j.find("mCamHeight");
	if(it_mCamHeight != j.end()) mCamHeight = (*it_mCamHeight).get<float>();
	auto it_mCamDist_X = j.find("mCamDist_X");
	if(it_mCamDist_X != j.end()) mCamDist_X = (*it_mCamDist_X).get<float>();
	auto it_mCamDist_Z = j.find("mCamDist_Z");
	if(it_mCamDist_Z != j.end()) mCamDist_Z = (*it_mCamDist_Z).get<float>();
	auto it_mPlayer1 = j.find("mPlayer1");
	if(it_mPlayer1 != j.end()) mPlayer1 = (*it_mPlayer1).get<bool>();
}
#ifdef _EDITOR_
void Gameplay ::PlayerCam::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"mCamHeight", &mCamHeight};
	_UIDraw << std::pair{"mCamDist_X", &mCamDist_X};
	_UIDraw << std::pair{"mCamDist_Z", &mCamDist_Z};
	_UIDraw << std::pair{"mPlayer1", &mPlayer1};
}
#endif

void Gameplay ::FloatAnim::ToJson(Gameplay ::FloatAnim::json& j) const {
	j["mIsPlaying"] = mIsPlaying;
	j["mSpeed"] = mSpeed;
	j["mRadius"] = mRadius;
}
void Gameplay ::FloatAnim::FromJson(const Gameplay ::FloatAnim::json& j) {
	auto it_mIsPlaying = j.find("mIsPlaying");
	if(it_mIsPlaying != j.end()) mIsPlaying = (*it_mIsPlaying).get<bool>();
	auto it_mSpeed = j.find("mSpeed");
	if(it_mSpeed != j.end()) mSpeed = (*it_mSpeed).get<float>();
	auto it_mRadius = j.find("mRadius");
	if(it_mRadius != j.end()) mRadius = (*it_mRadius).get<float>();
}
#ifdef _EDITOR_
void Gameplay ::FloatAnim::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"mIsPlaying", &mIsPlaying};
	_UIDraw << std::pair{"mSpeed", &mSpeed};
	_UIDraw << std::pair{"mRadius", &mRadius};
}
#endif

void PressStartToContinue::ToJson(PressStartToContinue::json& j) const {
	j["mDebugMode"] = mDebugMode;
	j["mNextLevel"] = mNextLevel;
	j["mTextMessage"] = mTextMessage;
	j["mTextPosX"] = mTextPosX;
	j["mTextPosY"] = mTextPosY;
	j["mTextPosZ"] = mTextPosZ;
	j["mDelay"] = mDelay;
}
void PressStartToContinue::FromJson(const PressStartToContinue::json& j) {
	auto it_mDebugMode = j.find("mDebugMode");
	if(it_mDebugMode != j.end()) mDebugMode = (*it_mDebugMode).get<bool>();
	auto it_mNextLevel = j.find("mNextLevel");
	if(it_mNextLevel != j.end()) mNextLevel = (*it_mNextLevel).get<std::string>();
	auto it_mTextMessage = j.find("mTextMessage");
	if(it_mTextMessage != j.end()) mTextMessage = (*it_mTextMessage).get<std::string>();
	auto it_mTextPosX = j.find("mTextPosX");
	if(it_mTextPosX != j.end()) mTextPosX = (*it_mTextPosX).get<float>();
	auto it_mTextPosY = j.find("mTextPosY");
	if(it_mTextPosY != j.end()) mTextPosY = (*it_mTextPosY).get<float>();
	auto it_mTextPosZ = j.find("mTextPosZ");
	if(it_mTextPosZ != j.end()) mTextPosZ = (*it_mTextPosZ).get<float>();
	auto it_mDelay = j.find("mDelay");
	if(it_mDelay != j.end()) mDelay = (*it_mDelay).get<float>();
}
#ifdef _EDITOR_
void PressStartToContinue::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"mDebugMode", &mDebugMode};
	_UIDraw << std::pair{"mNextLevel", &mNextLevel};
	_UIDraw << std::pair{"mTextMessage", &mTextMessage};
	_UIDraw << std::pair{"mTextPosX", &mTextPosX};
	_UIDraw << std::pair{"mTextPosY", &mTextPosY};
	_UIDraw << std::pair{"mTextPosZ", &mTextPosZ};
	_UIDraw << std::pair{"mDelay", &mDelay};
}
#endif

void Gameplay ::Door::ToJson(Gameplay ::Door::json& j) const {
	j["mID"] = mID;
	j["mDisplacement"] = mDisplacement;
}
void Gameplay ::Door::FromJson(const Gameplay ::Door::json& j) {
	auto it_mID = j.find("mID");
	if(it_mID != j.end()) mID = (*it_mID).get<int>();
	auto it_mDisplacement = j.find("mDisplacement");
	if(it_mDisplacement != j.end()) mDisplacement = (*it_mDisplacement).get<float>();
}
#ifdef _EDITOR_
void Gameplay ::Door::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"mID", &mID};
	_UIDraw << std::pair{"mDisplacement", &mDisplacement};
}
#endif

void LevelBoundaries::ToJson(LevelBoundaries::json& j) const {
	j["showHit"] = showHit;
	j["showLines"] = showLines;
	j["rays"] = rays;
	j["color"] = color;
}
void LevelBoundaries::FromJson(const LevelBoundaries::json& j) {
	auto it_showHit = j.find("showHit");
	if(it_showHit != j.end()) showHit = (*it_showHit).get<bool>();
	auto it_showLines = j.find("showLines");
	if(it_showLines != j.end()) showLines = (*it_showLines).get<bool>();
	auto it_rays = j.find("rays");
	if(it_rays != j.end()) rays = (*it_rays).get<int>();
	auto it_color = j.find("color");
	if(it_color != j.end()) color = (*it_color).get<Engine::Graphics::Color>();
}
#ifdef _EDITOR_
void LevelBoundaries::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"showHit", &showHit};
	_UIDraw << std::pair{"showLines", &showLines};
	_UIDraw << std::pair{"rays", &rays};
	_UIDraw << std::pair{"color", &color};
}
#endif

void PropBlinker::ToJson(PropBlinker::json& j) const {
	j["mMaxTime"] = mMaxTime;
}
void PropBlinker::FromJson(const PropBlinker::json& j) {
	auto it_mMaxTime = j.find("mMaxTime");
	if(it_mMaxTime != j.end()) mMaxTime = (*it_mMaxTime).get<float>();
}
#ifdef _EDITOR_
void PropBlinker::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"mMaxTime", &mMaxTime};
}
#endif

void PropWires::ToJson(PropWires::json& j) const {
	j["mMaxTime"] = mMaxTime;
}
void PropWires::FromJson(const PropWires::json& j) {
	auto it_mMaxTime = j.find("mMaxTime");
	if(it_mMaxTime != j.end()) mMaxTime = (*it_mMaxTime).get<float>();
}
#ifdef _EDITOR_
void PropWires::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"mMaxTime", &mMaxTime};
}
#endif

void Gameplay ::Teleport::ToJson(Gameplay ::Teleport::json& j) const {
	j["mIsHub"] = mIsHub;
}
void Gameplay ::Teleport::FromJson(const Gameplay ::Teleport::json& j) {
	auto it_mIsHub = j.find("mIsHub");
	if(it_mIsHub != j.end()) mIsHub = (*it_mIsHub).get<bool>();
}
#ifdef _EDITOR_
void Gameplay ::Teleport::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"mIsHub", &mIsHub};
}
#endif

void Gameplay ::Zone::ToJson(Gameplay ::Zone::json& j) const {
	j["mID"] = mID;
	j["mTextName"] = mTextName;
}
void Gameplay ::Zone::FromJson(const Gameplay ::Zone::json& j) {
	auto it_mID = j.find("mID");
	if(it_mID != j.end()) mID = (*it_mID).get<int>();
	auto it_mTextName = j.find("mTextName");
	if(it_mTextName != j.end()) mTextName = (*it_mTextName).get<std::string>();
}
#ifdef _EDITOR_
void Gameplay ::Zone::OnEditor() {
	Editor::UIPropertyDraw _UIDraw;
	_UIDraw << std::pair{"mID", &mID};
	_UIDraw << std::pair{"mTextName", &mTextName};
}
#endif
