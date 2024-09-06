#include "Definitions.hpp"
#include "DialogManager.hpp"

void DialogManager::AddDialog(const Json::Value& _jsonData)
{
	std::unique_ptr<DialogMap> dialogMap = std::make_unique<DialogMap>(_jsonData);
	dialogsMap.push_back(std::move(dialogMap));
}

void DialogManager::Event(sf::Event& _event, Player& _player)
{
	for (auto& dialog : dialogsMap)
	{
		dialog->Event(_event, _player);
	}
}

void DialogManager::Update(Player& _player)
{
	for (auto& dialog : dialogsMap)
	{
		dialog->Update(_player);

		if (dialog->Ready())
		{
			const std::string& type = dialog->GetType();
			if (type == "THINKING")
			{
				// update pos
				sf::Vector2f boxPos = _player.getPosition();
				boxPos.y -= _player.GetLocalAABB().height - DIALOG_BOX_OFFSET;
				dialog->SetPos(boxPos);
				dialog->StartDialog(_player);
			}
			else if (type == "NPC")
			{
				if (lilboo != nullptr && lilboo->IsVisible())
				{
					lilboo->SetDialog(*dialog, _player);
					dialog->SetReady(false);
				}
				else
				{
					lilbooBox.AddBox(*dialog);
				}
			}
		}
	}

	lilbooBox.Update(_player);

	if (lilboo != nullptr)
	{
		lilboo->Update(_player);
	}
}

DialogMap& DialogManager::GetDialogMap(const int& _mapID)
{
	for (auto& dialog : dialogsMap)
	{
		if (dialog->GetId() == _mapID)
		{
			return *dialog;
		}
	}

	Debug.error("No dialog found");

	// juste pour que visual fasse pas chier avec ce warning
	DialogMap* dialogErr = new DialogMap("GNGNGN DORIAN IL FAIT MAL LE DIALOGUE ET DONC ON DOIT RETURN DE LA MERDE POUR CORRIGER UN WARNING VISUAL STUDIO GNGNGN (C PEUT ETRE ENZO O FINAL)");
	return *dialogErr;
}

void DialogManager::SetLilboo(const Json::Value& _mapNpcJson)
{
	lilboo = std::make_unique<Lilboo>(_mapNpcJson);
}

void DialogManager::Clear(void)
{
	lilbooBox.Clear();
	dialogsMap.clear();

	lilboo.reset();
	lilboo = nullptr;
}