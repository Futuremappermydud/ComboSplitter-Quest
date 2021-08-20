#include "main.hpp"

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/XR/XRNode.hpp"
#include "GlobalNamespace/BeatmapObjectManager.hpp"
#include "GlobalNamespace/GameplayCoreInstaller.hpp"
#include "GlobalNamespace/ScoreController.hpp"
#include "GlobalNamespace/GameplayCoreSceneSetupData.hpp"
#include "GlobalNamespace/ComboUIController.hpp"
#include "GlobalNamespace/PlayerHeadAndObstacleInteraction.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/PauseMenuManager.hpp"
#include "GlobalNamespace/NoteController.hpp"
#include "GlobalNamespace/NoteCutInfo.hpp"
#include "GlobalNamespace/BeatmapObjectManager_NoteWasCutDelegate.hpp"
#include "GlobalNamespace/ColorType.hpp"
#include "GlobalNamespace/ResultsViewController.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/BeatmapData.hpp"
#include "GlobalNamespace/ObstacleController.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/ColorExtensions.hpp"
#include "GlobalNamespace/BeatmapLineData.hpp"
#include "GlobalNamespace/MultiplayerController.hpp"
#include "GlobalNamespace/BeatmapObjectData.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "GlobalNamespace/BeatmapObjectType.hpp"
#include "GlobalNamespace/StandardLevelFinishedController.hpp"
#include "TMPro/TextAlignmentOptions.hpp"
#include "TMPro/FontStyles.hpp"
#include "Polyglot/Localization.hpp"
#include "Polyglot/LocalizationImporter.hpp"
#include "Polyglot/Language.hpp"
#include "Polyglot/LocalizedTextMeshPro.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "System/Collections/Generic/List_1.hpp"
#include "System/Action_1.hpp"
#include "System/Collections/Generic/Dictionary_2.hpp"
#include "questui/shared/BeatSaberUI.hpp"

DECLARE_CLASS_CODEGEN(ComboSplitter, CustomComboUIController, UnityEngine::MonoBehaviour,
	DECLARE_INSTANCE_FIELD(GlobalNamespace::ComboUIController*, _comboPanel);
	DECLARE_INSTANCE_FIELD(GlobalNamespace::ColorScheme*, _scheme);
	DECLARE_INSTANCE_FIELD(System::Collections::Generic::List_1<HMUI::CurvedTextMeshPro*>*, handTexts);
	DECLARE_INSTANCE_FIELD(GlobalNamespace::PlayerHeadAndObstacleInteraction*, _interaction);
	DECLARE_INSTANCE_FIELD(GlobalNamespace::BeatmapObjectManager*, _manager);
	DECLARE_INSTANCE_FIELD(GlobalNamespace::PauseMenuManager*, _pauseManager);
	DECLARE_INSTANCE_FIELD(int, leftCombo);	
	DECLARE_INSTANCE_FIELD(int, rightCombo);
	DECLARE_INSTANCE_FIELD(int, maxLeftCombo);	
	DECLARE_INSTANCE_FIELD(int, maxRightCombo);
	DECLARE_INSTANCE_FIELD(int, totalLeftHits);	
	DECLARE_INSTANCE_FIELD(int, totalRightHits);
	DECLARE_INSTANCE_FIELD(int, _maxLeftNotes);	
	DECLARE_INSTANCE_FIELD(int, _maxRightNotes);
	DECLARE_INSTANCE_METHOD(void, Initialize, GlobalNamespace::BeatmapObjectManager* manager, GlobalNamespace::ComboUIController* comboPanel, GlobalNamespace::ColorScheme* scheme, GlobalNamespace::PlayerHeadAndObstacleInteraction* interaction, GlobalNamespace::PauseMenuManager* pauseManager, int maxLeftNotes, int maxRightNotes);
	DECLARE_INSTANCE_METHOD(void, Update);
	DECLARE_INSTANCE_METHOD(void, UpdatePanels);
	DECLARE_INSTANCE_METHOD(void, HandleNoteWasMissedEvent, GlobalNamespace::NoteController* noteController);

	void HandleNoteWasCutEvent(GlobalNamespace::NoteController* noteController, GlobalNamespace::NoteCutInfo& noteCutInfo);
)