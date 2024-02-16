#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "Map.h"
#include "MatrixTrans.h"
#include "Player.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete PlayerModel_;
	delete MapModel_;
	delete debugCamera_;
	delete mapModel_;
	delete Clearmodel_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugCamera_ = new DebugCamera(-18, 18);

	PlayerModel_ = (Model::CreateFromOBJ("float", true));

	model_ = Model::Create();

	// worldTransform_.Initialize();

	MapModel_ = (Model::CreateFromOBJ("Map", true));
	Mapmodel_ = Model::Create();

	mapModel_ = (Model::CreateFromOBJ("Item", true));
	mapmodel_ = Model::Create();

	Clearmodel_ = (Model::CreateFromOBJ("Item2", true));
	clearmodel_ = Model::Create();

	// MapworldTransform_.Initialize();
	//  ビュープロジェクション
	viewProjection_.Initialize();

	map_ = new Map();
	map_->Initialize(MapModel_, mapModel_, Clearmodel_);

	// 自キャラの生成
	player_ = new Player();

	// 自キャラの初期化
	player_->Initialize(PlayerModel_, map_);
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();
	map_->Update();
	debugCamera_->Update();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_P)) {
		isDebugCameraActive_ = true;
	}
#endif
	// カメラ
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

	// 仮のシーン切り替え
	if (player_->GetClear() == true) {
		GameSceneEnd_ = true;
	}

	viewProjection_.translation_.x = player_->GetX();
	//viewProjection_.translation_.y = player_->GetY();

	viewProjection_.UpdateMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラの描画
	player_->Draw(viewProjection_);
	map_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
