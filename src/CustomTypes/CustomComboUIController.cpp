#include "CustomTypes/CustomComboUIController.hpp"

//using namespace il2cpp_utils;
using namespace GlobalNamespace;
using namespace UnityEngine;
DEFINE_TYPE(ComboSplitter, CustomComboUIController);

void ComboSplitter::CustomComboUIController::Initialize(BeatmapObjectManager* manager, ComboUIController* comboPanel, ColorScheme* scheme, PlayerHeadAndObstacleInteraction* interaction, PauseMenuManager* pauseManager, int maxLeftNotes, int maxRightNotes)
{
	_comboPanel = comboPanel;
	_scheme = scheme;
	_interaction = interaction;
	_manager = manager;
	_pauseManager = pauseManager;

	leftCombo = 0;
	rightCombo = 0;

	maxLeftCombo = 0;
	maxRightCombo = 0;

	totalLeftHits = 0;
	totalRightHits = 0;

	_maxLeftNotes = maxLeftNotes;
	_maxRightNotes = maxRightNotes;

	handTexts = System::Collections::Generic::List_1<HMUI::CurvedTextMeshPro*>::New_ctor();

	auto leftText = _comboPanel->get_transform()->Find(il2cpp_utils::createcsstr("ComboCanvas/NumText"))->GetComponent<HMUI::CurvedTextMeshPro*>();
	leftText->set_name(il2cpp_utils::createcsstr("LeftHandComboText"));
	leftText->set_alignment(TMPro::TextAlignmentOptions::Left);
	leftText->set_text(il2cpp_utils::createcsstr(std::to_string(leftCombo)));
	leftText->set_color(scheme->get_saberBColor());

	auto rightText = GameObject::Instantiate(leftText, leftText->get_transform()->get_parent());
	rightText->set_name(il2cpp_utils::createcsstr("RightHandComboText"));
	rightText->set_alignment(TMPro::TextAlignmentOptions::Right);
	rightText->set_text(il2cpp_utils::createcsstr(std::to_string(rightCombo)));
	rightText->set_color(scheme->get_saberAColor());

	handTexts->Add(leftText);
	handTexts->Add(rightText);

	auto relPosTrans = _comboPanel->get_transform()->Find(il2cpp_utils::createcsstr("ComboText"))->get_transform();

	std::string spacerBase64 = R"(iVBORw0KGgoAAAANSUhEUgAAAAcAAABrCAYAAAC7ZJjyAAAATklEQVRIie3XMQ6AMAwEwTWigP//k9blRZYiAVEoqXLbjtyfQ9IFnIC4CyD3DgeTtuHimQo/Wx6NRqPRaDQajca/sQb9rKiRn13eHwBkA04aC8RfeX6jAAAAAElFTkSuQmCC)";
	
	auto spacer = QuestUI::BeatSaberUI::CreateImage(leftText->get_transform()->get_parent(), QuestUI::BeatSaberUI::Base64ToSprite(spacerBase64), Vector2(0.0f, 0.0f), Vector2(3.0f, 30.0f));
	if(false)
	{
		spacer->get_transform()->set_position(Vector3(relPosTrans->get_position().x-0.5f, relPosTrans->get_position().y, relPosTrans->get_position().z));
	}
	else
	{
		spacer->get_transform()->set_position(Vector3(relPosTrans->get_position().x, relPosTrans->get_position().y - 0.26f, relPosTrans->get_position().z));
	}
	spacer->get_transform()->set_localScale(Vector3(1.0f, 1.0f, 1.0f));
	spacer->get_gameObject()->set_layer(5);
	spacer->set_color(GlobalNamespace::ColorExtensions::ColorWithAlpha(Color::get_white(), 0.75f));

	//if(false)
	//{
		leftText->get_transform()->set_position(Vector3(relPosTrans->get_position().x + 0.69f, spacer->get_transform()->get_position().y + 0.15f, relPosTrans->get_position().z));
		rightText->get_transform()->set_position(Vector3(relPosTrans->get_position().x - 0.77f, spacer->get_transform()->get_position().y + 0.15f, relPosTrans->get_position().z));
	//}
	//else
	//{
	//	leftText->get_transform()->set_position(Vector3(relPosTrans->get_position().x, spacer->get_transform()->get_position().y + 0.25f, relPosTrans->get_position().z));
	//	rightText->get_transform()->set_position(Vector3(relPosTrans->get_position().x, spacer->get_transform()->get_position().y + 0.25f, relPosTrans->get_position().z));
	//}

	manager->add_noteWasCutEvent(il2cpp_utils::MakeDelegate<GlobalNamespace::BeatmapObjectManager::NoteWasCutDelegate*>(classof(GlobalNamespace::BeatmapObjectManager::NoteWasCutDelegate*), this, +[](CustomComboUIController* instance, GlobalNamespace::NoteController* noteController, GlobalNamespace::NoteCutInfo& noteCutInfo)
	{
		instance->HandleNoteWasCutEvent(noteController, noteCutInfo);
	}));

	manager->add_noteWasMissedEvent(il2cpp_utils::MakeDelegate<System::Action_1<NoteController*>*>(classof(System::Action_1<NoteController*>*), this, +[](CustomComboUIController* instance, GlobalNamespace::NoteController* noteController)
	{
		instance->HandleNoteWasMissedEvent(noteController);
	}));

	UpdatePanels();
}

void ComboSplitter::CustomComboUIController::Update()
{
	if (_interaction->get_intersectingObstacles()->get_Count() > 0 && !_pauseManager->get_enabled() && (leftCombo != 0 || rightCombo != 0))
	{
		rightCombo = 0;
		leftCombo = 0;
		UpdatePanels();
	}
}

void ComboSplitter::CustomComboUIController::HandleNoteWasCutEvent(NoteController* noteController, NoteCutInfo& noteCutInfo)
{
	if (noteController->get_noteData()->colorType == ColorType::ColorA) 
	{
		rightCombo++; 
		totalRightHits++;
	}
	else if (noteController->get_noteData()->colorType == ColorType::ColorB) 
	{
		leftCombo++; 
		totalLeftHits++;
	}

	if (noteController->get_noteData()->colorType == ColorType::None || !noteCutInfo.get_allIsOK())
	{
		leftCombo = 0;
		rightCombo = 0;
	}
	UpdatePanels();
}

void ComboSplitter::CustomComboUIController::HandleNoteWasMissedEvent(NoteController* noteController)
{
	if (noteController->get_noteData()->colorType == ColorType::ColorA) rightCombo = 0;
	else if (noteController->get_noteData()->colorType == ColorType::ColorB) leftCombo = 0;
	UpdatePanels();
}

void ComboSplitter::CustomComboUIController::UpdatePanels()
{
	maxLeftCombo = std::max(maxLeftCombo, leftCombo);
	maxRightCombo = std::max(maxRightCombo, rightCombo);
	handTexts->items->values[0]->set_text(il2cpp_utils::createcsstr(std::to_string(leftCombo)));
	handTexts->items->values[1]->set_text(il2cpp_utils::createcsstr(std::to_string(rightCombo)));
}