#include "pch.h"
#include "PlayerController.h"

#include "Game/GameMode.h"
#include "Game/GameState.h"

#include "Game/Authority/GameMoveValidator.h"

namespace chessy
{
	//////////////////////////////////////////////////////////////////////////////////////////
	PlayerController::PlayerController(std::shared_ptr<GameMode> gameMode) :
		m_gameMode(gameMode)
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void PlayerController::Play(const Position& input)
	{
		ReturnUnless(input.IsValid());

		switch (m_gameMode->GetState())
		{
			case EGameState::Gameplay:
			case EGameState::Check:
			{
				bool endTurn = Turn(m_gameMode->GetTurn(), input);

				if (endTurn)
				{
					m_gameMode->OnEndTurn();
				}
				break;
			}
			case EGameState::Promote:
			{
				m_gameMode->PromotePawn(input);
				m_gameMode->OnEndTurn();
				break;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////
	bool PlayerController::Turn(const EColor player, const Position& input)
	{
		AssertReturnUnless(input.IsValid(), false);
		//log position

		const auto& selected = m_select;
		if (!selected)
		{
			ReturnUnless(m_gameMode->GetValidator().CanMove(player, input), false);

			m_select.emplace(input);
			return false;//selecting a piece doesn't end your turn
		}

		//log selected
		if (*selected == input)
		{
			m_select.reset();
			return false;//unselecting a piece doesn't end your turn
		}
		else if (m_gameMode->GetValidator().IsPossibleMove(*selected, input))
		{
			m_gameMode->MoveChessPiece(*selected, input);
		}
		else
		{
			//log error
			return false;
		}

		//OnEndTurn
		m_select.reset();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////

	const std::optional<Position>& PlayerController::GetSelected() const
	{
		return m_select;
	}
}