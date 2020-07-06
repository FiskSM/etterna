#ifndef PlayerStageStats_H
#define PlayerStageStats_H

#include "Grade.h"
#include "HighScore.h"
#include "Etterna/Models/NoteData/NoteDataStructures.h"
#include "PlayerNumber.h"
#include "RadarValues.h"
#include "NoteTypes.h"
#include <map>

class Steps;
class Style;
struct lua_State;

/** @brief Contains statistics for one stage of play - either one song, or a
 * whole course. */
class PlayerStageStats
{
  public:
	/** @brief Set up the PlayerStageStats with default values. */
	PlayerStageStats() { InternalInit(); }
	void InternalInit();
	void Init(PlayerNumber pn);
	void Init(MultiPlayer pn);

	vector<NoteInfo> serializednd;

	/**
	 * @brief Add stats from one PlayerStageStats to another.
	 * @param other the other stats to add to this one. */
	void AddStats(const PlayerStageStats& other); // accumulate

	static Grade GetGrade(float p);
	[[nodiscard]] Grade GetGrade() const;
	static float MakePercentScore(int iActual, int iPossible);
	static std::string FormatPercentScore(float fPercentScore);
	// Calculate the difficulty rating for a specific score obtained by a player
	// - Mina
	Grade GetWifeGrade();
	[[nodiscard]] vector<float> CalcSSR(float ssrpercent) const;
	void GenerateValidationKeys(HighScore& hs) const;
	[[nodiscard]] float GetPercentDancePoints() const;
	[[nodiscard]] float GetWifeScore() const;
	[[nodiscard]] float GetCurWifeScore() const;
	[[nodiscard]] float GetMaxWifeScore() const;
	[[nodiscard]] float GetTimingScale() const;
	[[nodiscard]] vector<float> GetOffsetVector() const;
	[[nodiscard]] vector<int> GetNoteRowVector() const;
	[[nodiscard]] vector<int> GetTrackVector() const;
	[[nodiscard]] vector<TapNoteType> GetTapNoteTypeVector() const;
	[[nodiscard]] vector<HoldReplayResult> GetHoldReplayDataVector() const;
	[[nodiscard]] float GetCurMaxPercentDancePoints() const;

	[[nodiscard]] int GetLessonScoreActual() const;
	[[nodiscard]] int GetLessonScoreNeeded() const;
	void ResetScoreForLesson();

	bool m_for_multiplayer;
	PlayerNumber m_player_number;
	MultiPlayer m_multiplayer_number;
	const Style* m_pStyle;

	bool m_bJoined;
	bool m_bPlayerCanAchieveFullCombo;
	vector<Steps*> m_vpPossibleSteps;
	int m_iStepsPlayed; // how many of m_vpPossibleStepshow many of
						// m_vpPossibleSteps were played
	/**
	 * @brief How far into the music did the Player last before failing?
	 *
	 * This is updated by Gameplay, and scaled by the music rate. */
	float m_fAliveSeconds;

	/**
	 * @brief Have the Players failed at any point during the song?
	 *
	 * If FAIL_OFF is in use, this is always false.
	 *
	 * If health recovery is possible after failing (requires two players),
	 * this is only set if both players were failing at the same time. */
	bool m_bFailed;

	int m_iPossibleDancePoints;
	int m_iCurPossibleDancePoints;
	int m_iActualDancePoints;
	int m_iPossibleGradePoints;
	float m_fWifeScore;
	float CurWifeScore;
	float MaxWifeScore;
	float m_fTimingScale;
	vector<HoldReplayResult> m_vHoldReplayData;
	vector<float> m_vOffsetVector;
	vector<int> m_vNoteRowVector;
	vector<TapNoteType> m_vTapNoteTypeVector;
	vector<int> m_vTrackVector;
	vector<float> InputData;
	int m_iTapNoteScores[NUM_TapNoteScore];
	int m_iHoldNoteScores[NUM_HoldNoteScore];
	/** @brief The Player's current combo. */
	unsigned int m_iCurCombo;
	/** @brief The Player's max combo. */
	unsigned int m_iMaxCombo;
	/** @brief The Player's current miss combo. */
	unsigned int m_iCurMissCombo;
	int m_iCurScoreMultiplier;
	/** @brief The player's current score. */
	unsigned int m_iScore;
	/** @brief The theoretically highest score the Player could have at this
	 * point. */
	unsigned int m_iCurMaxScore;
	/** @brief The maximum score the Player can get this goaround. */
	unsigned int m_iMaxScore;

	/**
	 * @brief The possible RadarValues for a song.
	 *
	 * This is filled in by ScreenGameplay on the start of the notes. */
	RadarValues m_radarPossible;
	RadarValues m_radarActual;
	/** @brief How many songs were passed by the Player? */
	int m_iSongsPassed;
	/** @brief How many songs were played by the Player? */
	int m_iSongsPlayed;
	/**
	 * @brief How many seconds were left for the Player?
	 *
	 * This is used in the Survival mode. */
	float m_fLifeRemainingSeconds;

	// workout
	float m_iNumControllerSteps;

	bool everusedautoplay;
	bool luascriptwasloaded;
	bool filehadnegbpms; // the call after gameplay is over is apparently
						 // unreliable -mina
	bool filegotmines;	 // this needs to be set before any notedata transforms
	bool filegotholds;
	bool gaveuplikeadumbass; // flag 'giving up' status so i can flag it as
							 // failing so i dont have to remove the feature
							 // entirely -mina

	map<float, float> m_fLifeRecord;
	void SetLifeRecordAt(float fLife, float fStepsSecond);
	void GetLifeRecord(float* fLifeOut,
					   int iNumSamples,
					   float fStepsEndSecond) const;
	[[nodiscard]] float GetLifeRecordAt(float fStepsSecond) const;
	[[nodiscard]] float GetLifeRecordLerpAt(float fStepsSecond) const;
	[[nodiscard]] float GetCurrentLife() const;

	map<float, float> WifeRecord;
	void SetWifeRecordAt(float Wife, float fStepsSecond);
	void GetWifeRecord(float* WifeOut,
					   int iNumSamples,
					   float fStepsEndSecond) const;
	[[nodiscard]] float GetWifeRecordAt(float fStepsSecond) const;
	[[nodiscard]] float GetWifeRecordLerpAt(float fStepsSecond) const;

	struct Combo_t
	{
		// Update GetComboList in PlayerStageStats.cpp when adding new members
		// that should be visible from the Lua side.
		/**
		 * @brief The start time of the combo.
		 *
		 * This uses the same scale as the combo list mapping. */
		float m_fStartSecond{ 0 };
		/**
		 * @brief The size time of the combo.
		 *
		 * This uses the same scale as the life record. */
		float m_fSizeSeconds{ 0 };

		/** @brief The size of the Combo, in steps. */
		int m_cnt{ 0 };

		/**
		 * @brief The size of the combo that didn't come from this stage.
		 *
		 * This is generally rolled over from the last song.
		 * It is also a subset of m_cnt. */
		int m_rollover{ 0 };

		/**
		 * @brief Retrieve the size of the combo that came from this song.
		 * @return this song's combo size. */
		[[nodiscard]] int GetStageCnt() const { return m_cnt - m_rollover; }

		Combo_t() = default;
		[[nodiscard]] bool IsZero() const { return m_fStartSecond < 0; }
	};
	vector<Combo_t> m_ComboList;
	float m_fFirstSecond;
	float m_fLastSecond;

	[[nodiscard]] int GetComboAtStartOfStage() const;
	[[nodiscard]] bool FullComboOfScore(TapNoteScore tnsAllGreaterOrEqual) const;
	[[nodiscard]] bool FullCombo() const { return FullComboOfScore(TNS_W3); }
	[[nodiscard]] TapNoteScore GetBestFullComboTapNoteScore() const;
	[[nodiscard]] bool SingleDigitsOfScore(TapNoteScore tnsAllGreaterOrEqual) const;
	[[nodiscard]] bool OneOfScore(TapNoteScore tnsAllGreaterOrEqual) const;
	[[nodiscard]] int GetTotalTaps() const;
	[[nodiscard]] float GetPercentageOfTaps(TapNoteScore tns) const;
	void UpdateComboList(float fSecond, bool rollover);
	[[nodiscard]] Combo_t GetMaxCombo() const;

	[[nodiscard]] float GetSurvivalSeconds() const
	{
		return m_fAliveSeconds + m_fLifeRemainingSeconds;
	}

	int m_iPersonalHighScoreIndex;
	int m_iMachineHighScoreIndex;
	bool m_bDisqualified;
	[[nodiscard]] bool IsDisqualified() const;

	void UnloadReplayData(); // i don't really trust the deconstructors here,
							 // also prefer flexibility in this -mina
	HighScore m_HighScore;

	// Lua
	void PushSelf(lua_State* L);
};

#endif
