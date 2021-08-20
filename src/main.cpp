#include "main.hpp"
#include "CustomTypes/CustomComboUIController.hpp"
#include "GlobalNamespace/LevelCompletionResults.hpp"
#include <map>

static ModInfo modInfo;

using namespace GlobalNamespace;
using namespace UnityEngine;
using namespace ComboSplitter;
using namespace Polyglot;
using il2cpp_utils::createcsstr;

Configuration& getConfig() {
	static Configuration config(modInfo);
	config.Load();
	return config;
}

Logger& getLogger() {
	static Logger* logger = new Logger(modInfo);
	return *logger;
}

CustomComboUIController* comboUIController;


MAKE_HOOK_MATCH(ComboUIController_Start, &ComboUIController::Start, void, ComboUIController* self) {
	CustomComboUIController* swag = self->get_gameObject()->AddComponent<CustomComboUIController*>();
	comboUIController = swag;
	//BeatmapObjectManager* shit = Resources::FindObjectsOfTypeAll<BeatmapObjectManager*>()->values[0];
	GameplayCoreInstaller* shit2 = Resources::FindObjectsOfTypeAll<GameplayCoreInstaller*>()->values[0];
	ScoreController* shit3 = Resources::FindObjectsOfTypeAll<ScoreController*>()->values[0];
	PauseMenuManager* shit4 = Resources::FindObjectsOfTypeAll<PauseMenuManager*>()->values[0];
	int maxLeftNotes = 0;
	int maxRightNotes = 0;
	BeatmapData* data = shit2->sceneSetupData->difficultyBeatmap->get_beatmapData();
	for (int i = 0; i < data->beatmapLinesData->get_Length(); i++)
	{
		for (int j = 0; j < data->beatmapLinesData->values[i]->beatmapObjectsData->get_Count(); j++)
		{
			BeatmapObjectData* beatmapObject = data->beatmapLinesData->values[i]->beatmapObjectsData->get_Item(j);
			if(beatmapObject->get_beatmapObjectType() == BeatmapObjectType::Note)
			{
				NoteData* data = reinterpret_cast<NoteData*>(beatmapObject);
				if(data->colorType == ColorType::ColorA) maxLeftNotes++;
				if(data->colorType == ColorType::ColorB) maxRightNotes++;
			}
		}
	}
	
	swag->Initialize(shit3->beatmapObjectManager, self, shit2->sceneSetupData->colorScheme, shit3->playerHeadAndObstacleInteraction, shit4, maxLeftNotes, maxRightNotes);
}

MAKE_HOOK_MATCH(ResultsViewController_DidActivate, &ResultsViewController::DidActivate, void, ResultsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
	ResultsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
	
	
	bool leftFullCombo = comboUIController->totalLeftHits == comboUIController->_maxLeftNotes;
	bool rightFullCombo = comboUIController->totalRightHits == comboUIController->_maxRightNotes;
	if(!firstActivation)
	{
		if(self->levelCompletionResults->levelEndStateType == LevelCompletionResults::LevelEndStateType::Failed) return;
	}
	auto goodCutsLeftTex = self->goodCutsPercentageText;
	auto goodCutsRightTex = goodCutsLeftTex;
	auto maxComboRight = self->comboText;
	if(firstActivation) 
	{
		goodCutsRightTex = GameObject::Instantiate(goodCutsLeftTex, goodCutsLeftTex->get_transform()->get_parent());
		goodCutsRightTex->set_fontSize(self->comboText->get_fontSize());
		goodCutsRightTex->get_transform()->Translate(Vector3(0.0f, -0.26f, 0.0f));
		goodCutsLeftTex->set_fontSize(self->comboText->get_fontSize());
		goodCutsLeftTex->get_transform()->Translate(Vector3(0.0f, 0.1f, 0.0f));
		goodCutsRightTex->set_name(il2cpp_utils::newcsstr("GoodCutsRightTxt"));
		maxComboRight = GameObject::Instantiate(self->comboText, self->comboText->get_transform()->get_parent());
		maxComboRight->get_transform()->Translate(Vector3(0.0f, -0.26f, 0.0f));
	}
	else
	{
		goodCutsRightTex = GameObject::Find(il2cpp_utils::newcsstr("GoodCutsRightTxt"))->GetComponent<TMPro::TextMeshProUGUI*>();
	}
	goodCutsLeftTex->set_text(createcsstr(std::to_string(comboUIController->totalLeftHits) + "/" + std::to_string(comboUIController->_maxLeftNotes)));
	goodCutsLeftTex->set_color(comboUIController->_scheme->get_saberAColor());

	self->comboText->set_text(createcsstr(leftFullCombo ? "Full Combo" : "Max Combo " + std::to_string(comboUIController->maxLeftCombo)));

	goodCutsRightTex->set_text(createcsstr(std::to_string(comboUIController->totalRightHits) + "/" + std::to_string(comboUIController->_maxRightNotes)));
	goodCutsRightTex->set_color(comboUIController->_scheme->get_saberBColor());

	maxComboRight->set_text(createcsstr(rightFullCombo ? "Full Combo" : "Max Combo " + std::to_string(comboUIController->maxRightCombo)));
}

/*
static std::vector<std::string> modWords = std::vector<std::string>({"LEFT_GOOD_CUTS", "RIGHT_GOOD_CUTS"});

static std::map<std::string, std::string> english = std::map<std::string, std::string>({ {"LEFT_GOOD_CUTS", "Left Good Cuts"}, {"RIGHT_GOOD_CUTS", "Right Good Cuts"}});
static std::map<std::string, std::string> french = std::map<std::string, std::string>({ {"LEFT_GOOD_CUTS", "Laissees Bonnes Coupes"}, {"RIGHT_GOOD_CUTS", "Droit Bonnes Coupes"}});
static std::map<std::string, std::string> german = std::map<std::string, std::string>({ {"LEFT_GOOD_CUTS", "Laissees Bonnes Coupes"}, {"RIGHT_GOOD_CUTS", "Droit Bonnes Coupes"}});
static std::map<std::string, std::string> spanish = std::map<std::string, std::string>({ {"LEFT_GOOD_CUTS", "Laissees Bonnes Coupes"}, {"RIGHT_GOOD_CUTS", "Droit Bonnes Coupes"}});
static std::map<std::string, std::string> japanese = std::map<std::string, std::string>({ {"LEFT_GOOD_CUTS", "Laissees Bonnes Coupes"}, {"RIGHT_GOOD_CUTS", "Droit Bonnes Coupes"}});
static std::map<std::string, std::string> korean = std::map<std::string, std::string>({ {"LEFT_GOOD_CUTS", "Laissees Bonnes Coupes"}, {"RIGHT_GOOD_CUTS", "Droit Bonnes Coupes"}});

MAKE_HOOK_MATCH(Localization_Get, &Localization::Get, Il2CppString*, Il2CppString* key, Language language) {
	Il2CppString* result = Localization_Get(key, language);
	std::string keycpp = to_utf8(csstrtostr(key));
	if(std::find(modWords.begin(), modWords.end(), keycpp) != modWords.end())
	{
		switch(language)
		{
			case Language::English:
				result = createcsstr(english[keycpp]);
				break;
			case Language::French:
				result = createcsstr(french[keycpp]);
				break;
			case Language::German:
				result = createcsstr(german[keycpp]);
				break;
			case Language::Spanish:
				result = createcsstr(spanish[keycpp]);
				break;
			case Language::Japanese:
				result = createcsstr(japanese[keycpp]);
				break;	
			case Language::Korean:
				result = createcsstr(korean[keycpp]);
				break;
		}
	}

	return result;
}
*/
// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
	info.id = ID;
	info.version = VERSION;
	modInfo = info;
	
	getConfig().Load();
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
	il2cpp_functions::Init();

	INSTALL_HOOK(getLogger(), ComboUIController_Start);
	INSTALL_HOOK(getLogger(), ResultsViewController_DidActivate);

	custom_types::Register::AutoRegister();
}