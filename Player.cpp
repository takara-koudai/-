#include "ImGuiManager.h"
#include <player.h>

Player::~Player() {}

void Player::Initialize(Model* model, Map* map) {
	input_ = Input::GetInstance();
	// NULLポインタチェック
	assert(model);
	model_ = model;
	map_ = map;
	worldTransform_.translation_ = {20.0f, 5.0f, 0.0f};
	worldTransform_.Initialize();
};

void Player::Update() {

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 行列を定数バッファに転送
	const float kClientVeloctiy = 0.2f;
	Vector3 move = {0, 0, 0};

	if (ItemFlag == false) {
		ItemTime = 180;
	}

	float x = worldTransform_.translation_.x;
	float y = worldTransform_.translation_.y;
	if (map_->CheckItem(x, y) == true) {
		ItemFlag = true;
	}

	if (ItemFlag == true) {
		ItemTime--;
	}

	if (ItemTime < 0) {
		ItemFlag = false;
	}

	if (map_->CheckClear(x, y) == true) {
		ClearFlag = true;
	}


	// ゲームパッド状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) 
	{
		const float kspeed = 0.2f;
		const float i = 0.0f;

		Vector3 move = {
		    (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kspeed, 0.0f,
		    (float)joyState.Gamepad.sThumbLY / SHRT_MAX * i
		};

		// 移動量に速さを反映
		move = Normalize(move);
		move = Multiply3(kspeed, move);

		
		//ブロックとの判定
		float x = worldTransform_.translation_.x + 0.5f;
		float y = worldTransform_.translation_.y;

		if (map_->CheckStage(x, y) == false) {

			worldTransform_.translation_.x += 0.5f;
		}

		float x2 = worldTransform_.translation_.x - 0.1f;
		float y2 = worldTransform_.translation_.y;

		if (map_->CheckStage(x2, y2) == false) {

			worldTransform_.translation_.x -= 0.5f;
		}

		// 移動
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);



		//ジャンプ
		if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_A && jumpFlag == false)
		{
			jumpFlag = true;
			jumpSpeed = 1.5;

			if (ItemFlag == true) 
			{
				jumpSpeed += 1.0;
			}
		}
		if (jumpFlag == true)
		{
			jumpSpeed -= (0.1f + worldTransform_.translation_.y / 1000);

			if (jumpSpeed > 0)
			{
				for (float i = 0; i < jumpSpeed; i += 0.1f)
				{
					float x = worldTransform_.translation_.x;
					float y = worldTransform_.translation_.y + 0.1f;

					if (map_->CheckStage(x, y) == false)
					{
						worldTransform_.translation_.y += 0.1f;
					} 
					else 
					{
						jumpSpeed = -0.1;
						break;
					}
				}
			} 
			else 
			{
				for (float i = jumpSpeed; i < 0; i += 0.1f)
				{
					float x = worldTransform_.translation_.x;
					float y = worldTransform_.translation_.y - 0.04f;

					if (map_->CheckStage(x, y) == false)
					{
						worldTransform_.translation_.y -= 0.04f;
					} 
					else 
					{
						jumpSpeed = 0;
						jumpFlag = false;
					}
				}
			}
		} 
		else 
		{
			for (float i = jumpSpeed; i < 0; i += 0.1f)
			{
				float x = worldTransform_.translation_.x;
				float y = worldTransform_.translation_.y - 0.1f;

				if (map_->CheckStage(x, y) == false)
				{
					jumpFlag = false;
					jumpSpeed = 0;
				} 
			}
		}
	}

	/*if (input_->PushKey(DIK_D)) {
		// move.x = kClientVeloctiy;

		float x = worldTransform_.translation_.x + 0.5f;
		float y = worldTransform_.translation_.y;

		if (map_->CheckStage(x, y) == false) {

			worldTransform_.translation_.x += 0.5f;
		}
	}
	if (input_->PushKey(DIK_A)) {
		// move.x = -kClientVeloctiy;

		float x = worldTransform_.translation_.x - 0.1f;
		float y = worldTransform_.translation_.y;

		if (map_->CheckStage(x, y) == false) {

			worldTransform_.translation_.x -= 0.5f;
		}
	}
	if (input_->PushKey(DIK_SPACE) && jumpFlag == false) {
		//	move.y = 0.1;

		jumpFlag = true;
		jumpSpeed = 1.5;

		if (ItemFlag == true) {
			jumpSpeed += 1.0;
		}
	}
	if (jumpFlag == true) {

		jumpSpeed -= (0.1f + worldTransform_.translation_.y / 1000);

		if (jumpSpeed > 0) {

			for (float i = 0; i < jumpSpeed; i += 0.1f) {

				float x = worldTransform_.translation_.x;
				float y = worldTransform_.translation_.y + 0.1f;

				if (map_->CheckStage(x, y) == false) {

					worldTransform_.translation_.y += 0.1f;
				} else 
				{
					jumpSpeed = -0.1;
					break;
				}
			}
		}
		else
		{
			for (float i = jumpSpeed; i < 0; i += 0.1f) {

				float x = worldTransform_.translation_.x;
				float y = worldTransform_.translation_.y - 0.04f;

				if (map_->CheckStage(x, y) == false) {

					worldTransform_.translation_.y -= 0.04f;
				}
				else
				{
					jumpSpeed = 0;
					jumpFlag = false;
				}
			}
		}
	}
	else
	{
		float x = worldTransform_.translation_.x;
		float y = worldTransform_.translation_.y - 0.1f;

		if (map_->CheckStage(x, y) == false) {

			jumpFlag = true;
			jumpSpeed = 0;
		}
	}*/

	// 限界座標
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;

	// 行列の更新
	worldTransform_.TransferMatrix();
	//worldTransform_.UpdateMatrix();

};

void Player::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection);
}
