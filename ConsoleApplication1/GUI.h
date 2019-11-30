#pragma once
#include "AllHeader.h"
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL2_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear.h"
#include "nuklear_glfw_gl2.h"
#include "PlayerData.h"
#include "Sprite.h"
#include "GUIStyle.h"
#include "NeonShooter.h"

class GUI
{
protected:
	GLFWwindow* window;
	struct nk_context *ctx;
	int m_width;
	int m_height;

	NeonShooter* m_neonShooter;
	PlayerData* playerData;
	PlayerData* enemyData;
	GameData* gameData;

	Texture resultScreenTexture = Texture("../Media/Grid1.png");
	Texture backgroundTexture = Texture("../Media/Background.png");
	struct nk_image resultScreen = nk_image_id(resultScreenTexture.m_textureID);
	struct nk_image backgroundScreen = nk_image_id(backgroundTexture.m_textureID);

public:

	GUI() {};

	GUI(GLFWwindow* win, NeonShooter* neon)
	{
		m_neonShooter = neon;
		playerData = &neon->playerObject.playerData;
		enemyData = &neon->enemyObject.playerData;
		gameData = &neon->gameData;

		window = win;
		/* GUI */
		ctx = nk_glfw3_init(window, NK_GLFW3_INSTALL_CALLBACKS);
		{
			struct nk_font_atlas *atlas;
			nk_glfw3_font_stash_begin(&atlas);
			//struct nk_font* font = nk_font_atlas_add_from_file(atlas, "../../../extra_font/DroidSans.ttf", 20.0f, 0); */
			struct nk_font* font = nk_font_atlas_add_default(atlas, 20.0f, 0);
			nk_glfw3_font_stash_end();
			nk_style_set_font(ctx, &font->handle);
		}
		SetGUIStyle(Theme::THEME_RED_CUSTOM_1);
		
		
	}

	void SetGUIStyle(Theme theme)
	{
		set_style(ctx, theme);
		if (theme == Theme::THEME_RED_CUSTOM_1)
		{
			ctx->style.button.text_normal = nk_rgba(0, 0, 0, 255);
			ctx->style.button.text_active = nk_rgba(0, 0, 0, 255);
			ctx->style.progress.rounding = 2.0f;
			ctx->style.progress.border = 1.0f;
			ctx->style.progress.border_color = nk_rgba(255, 0, 0, 255);
		}
		else if (theme == Theme::THEME_RED_CUSTOM_2)
		{
			ctx->style.button.text_normal = nk_rgba(0, 0, 0, 255);
			ctx->style.button.text_active = nk_rgba(0, 0, 0, 255);
			ctx->style.progress.rounding = 2.0f;
			ctx->style.progress.border = 1.0f;
			ctx->style.progress.border_color = nk_rgba(137, 182, 224, 245);
		}
	}

	void Button(bool inactiveBool)
	{
		static int inactive = (int)inactiveBool;
		nk_layout_row_dynamic(ctx, 30, 1);
		nk_checkbox_label(ctx, "Inactive", &inactive);

		nk_layout_row_static(ctx, 30, 80, 1);
		if (inactive) {
			struct nk_style_button button;
			button = ctx->style.button;
			ctx->style.button.normal = nk_style_item_color(nk_rgb(40, 40, 40));
			ctx->style.button.hover = nk_style_item_color(nk_rgb(40, 40, 40));
			ctx->style.button.active = nk_style_item_color(nk_rgb(40, 40, 40));
			ctx->style.button.border_color = nk_rgb(60, 60, 60);
			ctx->style.button.text_background = nk_rgb(60, 60, 60);
			ctx->style.button.text_normal = nk_rgb(60, 60, 60);
			ctx->style.button.text_hover = nk_rgb(60, 60, 60);
			ctx->style.button.text_active = nk_rgb(60, 60, 60);
			nk_button_label(ctx, "button");
			ctx->style.button = button;
		}
	}

	~GUI()
	{
		nk_glfw3_shutdown();
	}

	void DrawGameGUI()
	{
		nk_glfw3_new_frame();
		glfwGetFramebufferSize(window, &m_width, &m_height);
		/* GUI */
		if (nk_begin(ctx, "", nk_rect(0, 0, m_width, m_height), NK_WINDOW_NO_SCROLLBAR))
		{
			//ctx->style.window.background = nk_rgba(0, 0, 0, 0);

			static struct nk_color yellow = { 255, 255, 0,255 };
			static struct nk_color red = { 242,109,125,255 };
			static struct nk_color green = { 0,255,0,255 };
			static struct nk_color white = { 255,255,255,255 };
			static struct nk_color neonblue = { 0,255,255,255 };

			if (gameData->gameState == GameState::HOME)
			{
				struct nk_window *win;
				struct nk_rect bounds;
				win = ctx->current;

				nk_layout_space_begin(ctx, NK_STATIC, 0.0f, 1);
				nk_layout_space_push(ctx, nk_rect(0, 0, m_width, m_height));
				if (nk_widget(&bounds, ctx))
				{
					nk_draw_image(&win->buffer, bounds, &resultScreen, nk_rgba(255, 255, 255, 150));
				}


				nk_layout_row_dynamic(ctx, 120, 1);
				nk_layout_row_dynamic(ctx, 30, 1);
				nk_label_colored(ctx, "Neon Shooter", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE, neonblue);
				nk_label_colored(ctx, "by Teenathayaala Vaiyanan - 0118856", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE, white);
				nk_layout_row_dynamic(ctx, 30, 1);
				nk_layout_row_dynamic(ctx, 30, 3);
				nk_label_colored(ctx, "", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE, neonblue);
				if (nk_button_label(ctx, "Start"))
				{
					m_neonShooter->InitializeGameWorld();
					gameData->gameState = GameState::WAITING;
				}
				nk_label_colored(ctx, "", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE, neonblue);

				nk_layout_row_dynamic(ctx, 30, 3);
				nk_label_colored(ctx, "", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE, neonblue);
				if (nk_button_label(ctx, "Tutorial"))
				{
					m_neonShooter->InitializeTutorial();
				}
				nk_label_colored(ctx, "", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE, neonblue);

				
			}
			else if (gameData->gameState == GameState::WAITING)
			{
				nk_layout_row_dynamic(ctx, 240, 1);
				nk_label_colored(ctx, "Waiting for Players", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE, neonblue);
				nk_layout_row_dynamic(ctx, 30, 3);
				nk_label_colored(ctx, "", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE, neonblue);
				if (nk_button_label(ctx, "Home"))
				{
					m_neonShooter->ExitGameWorld();
				}
				nk_label_colored(ctx, "", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE, neonblue);
			}
			else if (gameData->gameState == GameState::TUTORIAL || gameData->gameState == GameState::GAME_WORLD)
			{
				static const float ratioOne[] = { 180 };
				static const float ratioTwo[] = { 180, 150 };
				static const float ratioThree[] = { 350, 300 };
				static const float ratioFour[] = { 200, 350 };
				static const float ratioFive[] = { 300, 300 };

				struct nk_color playerColor = nk_rgba(0, 0, 0, 0);
				struct nk_color enemyColor = nk_rgba(0, 0, 0, 0);
				if (playerData->playerType == PlayerType::PLAYER_1)
				{
					playerColor = green;
					enemyColor = yellow;
				}
				else if (playerData->playerType == PlayerType::PLAYER_2)
				{
					playerColor = yellow;
					enemyColor = green;
				}

				if (gameData->matchResult == MatchResult::NONE)
				{
					size_t playerHealth = playerData->health;
					int playerHealthLimit = playerData->maxHealth;

					int playerMissileTimerLimit = m_neonShooter->GetMissileTimerLimit() * 100000;
					size_t playerMissileTimer = playerMissileTimerLimit - m_neonShooter->GetMissileTimer() * 100000;


					size_t enemyHealth = enemyData->health;
					int enemyHealthLimit = enemyData->maxHealth;

					nk_layout_row_dynamic(ctx, 30, 7);

					if (nk_button_label(ctx, "Home"))
					{
						if (gameData->gameState == GameState::GAME_WORLD)
						{
							MyPhoton::getInstance().sendReset(false, true);
						}
						m_neonShooter->ExitGameWorld();
					}

					if (nk_button_label(ctx, "Restart"))
					{
						if (gameData->gameState == GameState::GAME_WORLD)
						{
							MyPhoton::getInstance().sendReset(true, false);
						}
						m_neonShooter->Restart();
					}

					nk_layout_row(ctx, nk_layout_format::NK_STATIC, 20.0f, 2, ratioTwo);
					SetGUIStyle(Theme::THEME_RED_CUSTOM_1);
					nk_label_colored(ctx, "Player Health: ", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, playerColor);
					nk_progress(ctx, &playerHealth, playerHealthLimit, nk_modify::NK_FIXED);
					SetGUIStyle(Theme::THEME_RED_CUSTOM_2);
					nk_layout_row(ctx, nk_layout_format::NK_STATIC, 20.0f, 2, ratioTwo);
					nk_label_colored(ctx, "Missile Reload: ", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, playerColor);
					nk_progress(ctx, &playerMissileTimer, playerMissileTimerLimit, nk_modify::NK_FIXED);
					nk_layout_row(ctx, nk_layout_format::NK_STATIC, 20.0f, 1, ratioOne);
					nk_label_colored(ctx, TextCombiner("Player Score: ", playerData->score, "").c_str(), NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, playerColor);

					nk_layout_row(ctx, nk_layout_format::NK_STATIC, 20.0f, 2, ratioTwo);
					SetGUIStyle(Theme::THEME_RED_CUSTOM_1);
					nk_label_colored(ctx, "Enemy Health: ", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, enemyColor);
					nk_progress(ctx, &enemyHealth, enemyHealthLimit, nk_modify::NK_FIXED);
					nk_layout_row(ctx, nk_layout_format::NK_STATIC, 20.0f, 1, ratioOne);
					nk_label_colored(ctx, TextCombiner("Enemy Score: ", enemyData->score, "").c_str(), NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, enemyColor);

					if (gameData->gameState == GameState::TUTORIAL)
					{
						nk_layout_row_dynamic(ctx, 30, 1);
						nk_label_colored(ctx, "WASD OR Arrow Key to Move", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, white);
						nk_label_colored(ctx, "Left Click for machine gun", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, white);
						nk_label_colored(ctx, "Right Click for missile", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, white);
						nk_label_colored(ctx, "Shoot at the missile to destroy it", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, white);
					}
				}
				else if (gameData->matchResult != MatchResult::NONE && gameData->winner != PlayerType::NO_PLAYER)
				{
					struct nk_window *win;
					struct nk_rect bounds;
					win = ctx->current;

					nk_layout_space_begin(ctx, NK_STATIC, 0.0f, 1);
					nk_layout_space_push(ctx, nk_rect(0, 0, m_width, m_height));
					if (nk_widget(&bounds, ctx))
					{
						nk_draw_image(&win->buffer, bounds, &backgroundScreen, nk_rgba(255, 255, 255, 150));
					}
					
					if (gameData->matchResult == MatchResult::WIN)
					{
						if (gameData->winner == playerData->playerType)
						{
							nk_layout_row(ctx, NK_STATIC, 30, 2, ratioThree);
							nk_label_colored(ctx, "", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE, neonblue);
							nk_label_colored(ctx, "You Win", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, playerColor);
						}
						else if (gameData->winner == enemyData->playerType)
						{
							nk_layout_row(ctx, NK_STATIC, 30, 2, ratioThree);
							nk_label_colored(ctx, "", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE, neonblue);
							nk_label_colored(ctx, "You Lose", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, playerColor);
						}
					}
					else if (gameData->matchResult == MatchResult::DRAW)
					{
						nk_layout_row(ctx, NK_STATIC, 30, 2, ratioFive);
						nk_label_colored(ctx, "", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE, neonblue);
						nk_label_colored(ctx, "The result is Draw", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, white);
					}

					nk_layout_row(ctx, NK_STATIC, 30, 2, ratioFour);
					nk_label_colored(ctx, "", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE, neonblue);
					nk_label_colored(ctx, "You can restart or return to home menu", NK_TEXT_ALIGN_LEFT | NK_TEXT_ALIGN_MIDDLE, white);
					nk_layout_row_dynamic(ctx, 30, 3);
					nk_label_colored(ctx, "", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE, neonblue);
					if (nk_button_label(ctx, "Home"))
					{
						if (gameData->gameState == GameState::GAME_WORLD)
						{
							MyPhoton::getInstance().sendReset(false, true);
						}
						m_neonShooter->ExitGameWorld();
					}
					nk_label_colored(ctx, "", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE, neonblue);

					nk_label_colored(ctx, "", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE, neonblue);
					if (nk_button_label(ctx, "Restart"))
					{
						if (gameData->gameState == GameState::GAME_WORLD)
						{
							MyPhoton::getInstance().sendReset(true, false);
						}
						m_neonShooter->Restart();
					}
					nk_label_colored(ctx, "", NK_TEXT_ALIGN_CENTERED | NK_TEXT_ALIGN_MIDDLE, neonblue);
				}
			}
		}
		
		nk_end(ctx);
		nk_glfw3_render(NK_ANTI_ALIASING_ON);	
	}

	static std::string TextCombiner(std::string text1, int num, std::string text2)
	{
		std::stringstream sstm;
		sstm << text1 << num << text2;
		return sstm.str();
	}
};
