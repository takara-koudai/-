#pragma once
#include "Input.h"
#include "MatrixTrans.h"
#include "Model.h"

#include "Map.h"

/// <summary>
/// 自キャラ
/// </summary>

class Player {

public:
	///< summary>
	/// デストラクタ
	///  <summary>
	~Player();

	///< summary>
	/// 攻撃
	///  <summary>

	///< summary>
	/// 初期化
	///</summary>
	void Initialize(Model* model, Map* map);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>

	void Draw(ViewProjection& viewProjection);

	float GetX() { return worldTransform_.translation_.x; }
	float GetY() { return worldTransform_.translation_.y; }

	bool GetClear() { return ClearFlag; }

	void Reset() {
		worldTransform_.translation_ = {20.0f, 5.0f, 0.0f};

		ClearFlag = false;
	}

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	Input* input_ = nullptr;

	Map* map_ = nullptr;

	bool jumpFlag = false;
	float jumpSpeed = 0;

	bool ItemFlag = false;
	float ItemTime = 180;

	bool ClearFlag = false;

	// ゲームパッドの状態を得る変数
	XINPUT_STATE joyState;

};
