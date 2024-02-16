#include "ImGuiManager.h"
#include <Map.h>

Map::~Map() {}

void Map::Initialize(Model* model, Model* model_, Model* Clearmodel) {
	input_ = Input::GetInstance();
	// NULLポインタチェック
	assert(model);
	Mapmodel = model;
	mapmodel = model_;
	clearmodel = Clearmodel;
	for (int y = 0; y < 30; y++) {
		for (int x = 0; x < 20; x++) {
			worldTransform_[y][x].translation_.x = (float)x * 2;
			worldTransform_[y][x].translation_.y = (float)y * 2;

			worldTransform_[y][x].Initialize();
		}
	}
};

void Map::Update() {
	// 変換行列を更新
	for (int y = 0; y < 30; y++) {
		for (int x = 0; x < 20; x++) {
			worldTransform_[y][x].matWorld_ = MakeAffineMatrix(
			    worldTransform_[y][x].scale_, worldTransform_[y][x].rotation_,
			    worldTransform_[y][x].translation_);
			// 行列の更新
			worldTransform_[y][x].TransferMatrix();
		}
	}
};

void Map::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	for (int y = 0; y < 30; y++) {
		for (int x = 0; x < 20; x++) {
			if (mapData[y][x] == 1) {
				Mapmodel->Draw(worldTransform_[y][x], viewProjection);
			}
			if (mapData[y][x] == 2) {
				mapmodel->Draw(worldTransform_[y][x], viewProjection);
			}
			if (mapData[y][x] == 3) {
				clearmodel->Draw(worldTransform_[y][x], viewProjection);
			}
		}
	}
}

// 移動チェック
bool Map::CheckStage(float px, float py) {

	for (int y = 0; y < 30; y++) {
		for (int x = 0; x < 20; x++) {
			if (mapData[y][x] == 1) {
				float x2 = worldTransform_[y][x].translation_.x;
				float y2 = worldTransform_[y][x].translation_.y;

				if (abs(x2 - px) < 1.9 && abs(y2 - py) < 1.9) {
					return true;
				}
			}

			/*if (mapData[y][x] == 2) {
			    float x2 = worldTransform_[y][x].translation_.x;
			    float y2 = worldTransform_[y][x].translation_.y;

			    if (abs(x2 - px) < 1.9 && abs(y2 - py) < 1.9) {
			        return true;
			    }
			}*/
		}
	}

	return false;
}

#pragma region アイテム

bool Map::CheckItem(float px, float py) {

	for (int y = 0; y < 30; y++) {
		for (int x = 0; x < 20; x++) {
			if (mapData[y][x] == 2) {
				float x2 = worldTransform_[y][x].translation_.x;
				float y2 = worldTransform_[y][x].translation_.y;

				if (abs(x2 - px) < 1.9 && abs(y2 - py) < 1.9) {
					return true;
				}
			}
		}
	}

	return false;
}
#pragma endregion

#pragma region クリア
bool Map::CheckClear(float px, float py) {

	for (int y = 0; y < 30; y++) {
		for (int x = 0; x < 20; x++) {
			if (mapData[y][x] == 3) {
				float x2 = worldTransform_[y][x].translation_.x;
				float y2 = worldTransform_[y][x].translation_.y;

				if (abs(x2 - px) < 1.9 && abs(y2 - py) < 1.9) {
					return true;
				}
			}
		}
	}

	return false;
}
#pragma endregion