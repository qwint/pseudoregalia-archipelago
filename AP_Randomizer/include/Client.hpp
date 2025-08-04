#pragma once

namespace Client {
	void Connect(const std::string, const std::string, const std::string);
	void SendCheck(int64_t);
	void Say(std::string);
	void PollServer();
	void SetZoneData();
	void CompleteGame();
	void SendDeathLink();
	void Disconnect();
	bool IsMissingLocation(int64_t);
	std::string GetPseudoItemName(int64_t);
	std::string GetLocationName(int64_t, int);
	std::string GetPlayerName(int);
}