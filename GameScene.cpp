#include "GameScene.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete spriteBG;
	for (int i = 0; i < _countof(object3d); i++)
	{
		delete object3d[i];
	}

	delete sprite1_;
	delete sprite2_;
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->input = input;

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/background.png");

	Sprite::LoadTexture(2, L"Resources/texture.png");

	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	for (int i = 0; i < _countof(object3d); i++)
	{
		// 3Dオブジェクト生成
		object3d[i] = Object3d::Create();
		object3d[i]->SetPosition(XMFLOAT3(-32 * i, 0, 0));
		object3d[0]->Update();

	}
	

	sprite1_ = Sprite::Create(2, { 0.0f,0.0f });
	sprite2_ = Sprite::Create(2, { 500,500 }, {1,0,0,1}, {0,0},false,true);
}

void GameScene::Update()
{
	// オブジェクト移動
	if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
	{
		for (int i = 0; i < _countof(object3d); i++)
		{
			// 現在の座標を取得
			XMFLOAT3 position = object3d[0]->GetPosition();

			// 移動後の座標を計算
			if (input->PushKey(DIK_UP)) { position.y += 1.0f; }
			else if (input->PushKey(DIK_DOWN)) { position.y -= 1.0f; }
			if (input->PushKey(DIK_RIGHT)) { position.x += 1.0f; }
			else if (input->PushKey(DIK_LEFT)) { position.x -= 1.0f; }

			// 座標の変更を反映
			object3d[0]->SetPosition(position);
		}
		
	}

	// カメラ移動
	if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A) || input->PushKey(DIK_E) || input->PushKey(DIK_Z))
	{
		if (input->PushKey(DIK_W)) { object3d[0]->CameraMoveEyeVector({ 0.0f,+3.0f,0.0f }); }
		else if (input->PushKey(DIK_S)) { object3d[0]->CameraMoveEyeVector({ 0.0f,-3.0f,0.0f }); }
		if (input->PushKey(DIK_D)) { object3d[0]->CameraMoveEyeVector({ +3.0f,0.0f,0.0f }); }
		else if (input->PushKey(DIK_A)) {object3d[0]->CameraMoveEyeVector({ -3.0f,0.0f,0.0f }); }
		if (input->PushKey(DIK_E)) { object3d[0]->CameraMoveEyeVector({ 0.0f,0.0f,+3.0f }); }
		else if (input->PushKey(DIK_Z)) { object3d[0]->CameraMoveEyeVector({ 0.0f,0.0f,-3.0f }); }
	}
	
	
	object3d[0]->Update();
	object3d[1]->UpdateMat();

	if (input->PushKey(DIK_SPACE))
	{
		//現在の座標の取得
		XMFLOAT2 position = sprite1_->GetPosition();
		//移動後の座標を計算
		position.x += 1.0f;
		sprite1_->SetPosition(position);
	}

}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	for (int i = 0; i < _countof(object3d); i++)
	{
		object3d[i]->Draw();

	}

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	/*sprite1_->Draw();
	sprite2_->Draw();*/

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
