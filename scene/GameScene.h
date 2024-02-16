#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Map.h"
#include "Model.h"
#include "Player.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DebugCamera.h>
#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {
	Sprite* sprite_ = nullptr;

	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	bool GameSceneEnd_ = false;

	bool IsSceneEnd() { return GameSceneEnd_; }
	//SCENE NextScene() { return SCENE::Clear; }

	bool ResetSceneEnd_ = false;

	void Start() { GameSceneEnd_ = false; }

	void playerReset() { player_->Reset(); }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	Model* PlayerModel_ = nullptr;
	Model* model_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	WorldTransform worldTransform_[5][10];
	Player* player_ = nullptr;
	Map* map_ = nullptr;

	// ステージ1
	Model* MapModel_ = nullptr;
	Model* Mapmodel_ = nullptr;
	// ステージ2
	Model* mapModel_ = nullptr;
	Model* mapmodel_ = nullptr;
	// ステージ3
	Model* Clearmodel_ = nullptr;
	Model* clearmodel_ = nullptr;
	// WorldTransform MapworldTransform_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
