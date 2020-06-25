#include <api/Loader.h>
#include <string>
#include <iostream>
#include <JsonLoader.h>
#include <time.h>
#include <sstream>
#include <chrono>
#include <stdio.h>
#include <fstream>
#include <future>
#include <stl/langpack.h>
#include "csvwriter.h"
using namespace std;
inline bool mode = false;
typedef signed int          INT32, * PINT32;

inline bool logCmdMlock = true;
inline bool CmdBlockWriteable = true;

inline bool LogUseItem = true;
inline bool LogChest = true;
inline bool LogPlace = true;
inline bool LogDestory = true;
inline bool LogCDim = true;
inline bool LogDie = true;

inline bool showLogUseItem = true;
inline bool showLogChest = true;
inline bool showLogBlock = true;
inline bool showLogCDim = true;
inline bool showLogDie = true;
inline bool logfd = true;
inline bool logfh = true;

inline bool LogCMD;
inline bool LogJoin;
inline bool LogLeft;
inline bool LogChat;
inline LangPack LP("langpack/behaviorlog.json");
inline string logfilec = "behaviorlog";
inline void loadconf() {
	ifstream cfile;
	cfile.open("config/behaviorlog.json");
	if (cfile)
	{
		cfile.close();
		try {
			ConfigJReader jr("config/behaviorlog.json");
			jr.bind("LogUseItem", LogUseItem, true);
			jr.bind("LogChest", LogChest, true);
			jr.bind("LogPlace", LogPlace, true);
			jr.bind("LogDestory", LogDestory, true);
			jr.bind("LogCDim", LogCDim, true);
			jr.bind("LogFD", logfd, true);
			jr.bind("LogFH", logfh, false);
			jr.bind("logfile", logfilec, (string)"behaviorlog");

			jr.bind("LogCMD", LogCMD, true);
			jr.bind("LogJoin", LogJoin, true);
			jr.bind("LogLeft", LogLeft, true);
			jr.bind("LogChat", LogChat, true);
			jr.bind("LogDie", LogDie, true);

			jr.bind("showLogUseItem", showLogUseItem, false);
			jr.bind("showLogChest", showLogChest, true);
			jr.bind("showLogBlock", showLogBlock, false);
			jr.bind("showLogCDim", showLogCDim, true);
			jr.bind("showLogDie", showLogDie, true);

			jr.bind("logCmdMlock", logCmdMlock, true);
			jr.bind("CmdBlockWriteable", CmdBlockWriteable, true);
			//printf("configloaded");
		}
		catch (string e) {
			printf("JSON ERROR %s\n", e.c_str());
			exit(1);
		}
	}
	else {
		cfile.close();
		cout << _TRS("no.config.usedefault") << endl;
	}
}
template <size_t size> void UtoA_Fill(char(&buf)[size], int num) {
	int nt = size - 1;
	buf[nt] = 0;
	for (auto i = nt - 1; i >= 0; --i) {
		char d = '0' + (num % 10);
		num /= 10;
		buf[i] = d;
	}
}

inline const string getday() {
	auto timet = chrono::system_clock::to_time_t(chrono::system_clock::now());
	tm time;
	char buf[3] = { 0 };
	localtime_s(&time, &timet);
	string str(to_string((time.tm_year + 1900)));
	UtoA_Fill(buf, time.tm_mon + 1);
	str += buf;
	UtoA_Fill(buf, time.tm_mday);
	str += buf;
	return str;
}
inline const string gethour() {
	auto timet = chrono::system_clock::to_time_t(chrono::system_clock::now());
	tm time;
	char buf[3] = { 0 };
	localtime_s(&time, &timet);
	string str;
	UtoA_Fill(buf, time.tm_hour);
	str += buf;
	return str;
}
inline const string gettime() {
	auto timet = chrono::system_clock::to_time_t(chrono::system_clock::now());
	tm time;
	char buf[3] = { 0 };
	localtime_s(&time, &timet);
	string str(to_string((time.tm_year + 1900)));
	str += "-";
	UtoA_Fill(buf, time.tm_mon + 1);
	str += buf; str += "-";
	UtoA_Fill(buf, time.tm_mday);
	str += buf; str += " ";
	UtoA_Fill(buf, time.tm_hour);
	str += buf; str += ":";
	UtoA_Fill(buf, time.tm_min);
	str += buf; str += ":";
	UtoA_Fill(buf, time.tm_sec);
	str += buf;
	return str;
}
inline const string getfilename() {
	string logfile = logfilec;
	if (logfd) {
		logfile += getday();
		if (logfh) {
			logfile += gethour();
		}
	}
	logfile += ".csv";
	return logfile;

}
//file write
inline void fw(const string filen, const string instr) {
	ofstream outfile;
	outfile.open(filen, ios::app);
	if (!outfile)
	{
		cout << "[" << gettime() << u8" INFO][BehaviorLog] " <<_TRS("log.failwritefile")<< endl;
	}
	outfile << instr << endl;
	outfile.close();
}
inline void wlog(const string time, const string title, const string player, const char stand, int dim, const string pos, const string operate, const string target) {
	string logfile = getfilename();
	string dims = to_string(dim);
	string inair = (!stand ? u8"A" : "G");
	CSVWriter csv(",");
	csv.addLine()
		<< time
		<< title
		<< player
		<< inair
		<< dims
		<< pos
		<< operate
		<< target;

	//string str = strings.str();
	fw(logfile, csv.getstr());
}
inline void wlog(const string time, const string title, const string player, int dim, const string pos, const string operate, const string target) {
	string logfile = getfilename();
	string dims = to_string(dim);

	CSVWriter csv(",");
	csv.addLine()
		<< time
		<< title
		<< player
		<< "" 
		<< dims
		<< pos
		<< operate
		<< target;
	fw(logfile, csv.getstr());
}

inline void wlog(const string time, const string title, const string player, const string operate, int dim, int slot, int count, const string obj) {
	string logfile = getfilename();
	string dims = to_string(dim);
	string counts = to_string(count);
	string slots = to_string(slot);
	CSVWriter csv(",");
	csv.addLine() << time
		<< title
		<< player
		<< operate
		<< dims
		<< slots
		<< counts
		<< obj;

	fw(logfile, csv.getstr());
}

inline void wlog(const string time, const string title, const string player, const string operate, int dim, const string text) {
	string logfile = getfilename();
	string dims = to_string(dim);
	CSVWriter csv(",");
	csv.addLine() << time
		<< title
		<< player
		<< operate
		<< dims
		<< ""
		<< ""
		<< text;

	fw(logfile, csv.getstr());
}
inline void wlog(const string time, const string title, const string player, int dim, const string ip, const string xuid) {
	string logfile = getfilename();
	string dims = to_string(dim);
	CSVWriter csv(",");
	csv.addLine()
		<< time
		<< title
		<< player
		<< dims
		<< "IP"
		<< ip
		<< "xuid"
		<< xuid;
	fw(logfile, csv.getstr());
}
inline void wlog(const string time, const string title, const string player, const string operate, const string pos, const string text) {
	string logfile = getfilename();
	CSVWriter csv(",");
	csv.addLine() << time
		<< title
		<< player
		<< "U"
		<< "IP"
		<< operate
		<< pos
		<< text;
	fw(logfile, csv.getstr());
}


inline const string Title(const string titll) {
	return "[" + gettime() + " " + titll + "][Log]";
}
inline const string Coordinator(INT32 coordinator[]) {
	return string("(") + to_string(coordinator[0]) + ", " + to_string(coordinator[1]) + ", " +
		to_string(coordinator[2]) + ")";
}

inline const string Pos(const Vec3 vec) {
	return "(" + to_string(int(vec.x)) + ", " + to_string(int(vec.y)) + ", " + to_string(int(vec.z)) + ")";
}

inline auto Dimension(int v) {
	switch (v) {
	case 0: return _TRS("Main");
	case 1: return _TRS("Nether");
	case 2: return _TRS("End");
	}
	return (string)"unknow";
}
inline auto posny(INT32 coordinator[]) {
	return string(to_string(coordinator[0]) + " " + to_string(coordinator[2]));
}


namespace logger {
	inline void Error(const string title, const string& player_name, int dimension, const string& content) {
		printf(_TR("log.Error"), Title(title).c_str(), player_name.c_str(), Dimension(dimension).c_str(), content.c_str());
	}
	inline void Block(
		const string title, const string player_name, char isStand, int dimension, const string operation,
		const string bname, INT32 pos[]) {
		auto block_name_inner = bname;
		if (block_name_inner == "") block_name_inner = _TRS("unknow.block");
		if (showLogBlock)
			printf(_TR("log.Block"), Title(title).c_str(), player_name.c_str(), (!isStand ? u8"(In Air)" : ""), Dimension(dimension).c_str(), Coordinator(pos).c_str(), operation.c_str(), block_name_inner.c_str());
		wlog(gettime(), title, player_name, isStand, dimension, Coordinator(pos), operation, block_name_inner);
	}
	inline void Item(
		const string title, const string& player_name, char isStand, int dimension, const string& operation,
		const string& item_name, INT32 coordinator[]) {
		if (showLogUseItem)
			printf(_TR("log.Item"), Title(title).c_str(), player_name.c_str(), (!isStand ? u8"(In Air)" : ""), Dimension(dimension).c_str(), Coordinator(coordinator).c_str(), operation.c_str(), item_name.c_str());
		wlog(gettime(), title, player_name, isStand, dimension, Coordinator(coordinator), operation, item_name);
	}
	inline void Interaction(
		const string title, const string& player_name, char isStand, int dimension, const string& operation,
		const string& object_name, INT32 coordinator[]) {
		if (showLogChest)
			printf(_TR("log.Interaction"), Title(title).c_str(), player_name.c_str(), (!isStand ? u8"(In Air)" : ""), operation.c_str(), Dimension(dimension).c_str(), Coordinator(coordinator).c_str(), object_name.c_str());
		wlog(gettime(), title, player_name, isStand, dimension, Coordinator(coordinator), operation, object_name);

	}
	inline void Container_In(
		const string title, const string player_name, int dimension, int slot, int count,
		const string object_name) {
		cout << Title(title).c_str() << player_name << slot << count << object_name << endl;
		if (showLogChest)
			printf(_TR("log.ContainerIn"), Title(title).c_str(), player_name.c_str(), to_string(slot).c_str(), to_string(count).c_str(), object_name.c_str());
		wlog(gettime(), title, player_name, "put", dimension, slot, count, object_name);
	}
	inline void Container_Out(const string title, const string player_name, int dimension, int slot) {
		if (showLogChest)
			printf(_TR("log.ContainerOut"), Title(title).c_str(), player_name.c_str(), to_string(slot).c_str());
		wlog(gettime(), title, player_name, "get", dimension, slot, -1, "unknow");
	}

	inline void ChangeDimension(const string title, const string player_name, int dimension, const Vec3 v) {
		if (showLogCDim)
			printf(_TR("log.Cdim"), Title(title).c_str(), player_name.c_str(), Dimension(dimension).c_str(), Pos(v).c_str());
		wlog(gettime(), title, player_name, dimension, Pos(v), "changedim", "");
	}

	inline void showDie(const string title, int dim, const Vec3 pos, const string mob_name, const string src_name) {
		if (showLogDie)
			printf(_TR("log.Die"), Title(title).c_str(), mob_name.c_str(), Dimension(dim).c_str(), Pos(pos).c_str(), src_name.c_str());
		wlog(gettime(), title, mob_name, dim, Pos(pos), "killed", src_name);
	}
	inline void CmdB(const string title, const string player_name, int dimension, const string content) {
		printf(_TR("log.CmdBlock"), Title(title).c_str(), player_name.c_str(), Dimension(dimension).c_str(), content.c_str());
	}
	inline void cmdbw(string title, const string operate, const string cmd, const string value) {
		printf(_TR("log.CmdBlockWrite"), Title(title).c_str(), operate.c_str(), cmd.c_str(), value.c_str());
		wlog(gettime(), title, "", "Write", value, cmd);
	}
	inline void cmdbm(string title, const string player, Vec3 pos, int dim) {
		printf(_TR("log.open.cmdblcok.minecrat"), Title(title).c_str(), player.c_str(), Dimension(dim).c_str(), Pos(pos).c_str());
		wlog(gettime(), title, player, "Open", Pos(pos), "CommandBlock_Minecrat");
	}
	inline void cmdbb(string title, const string player, const string bpos, int dim) {
		printf(_TR("log.open.cmdblcok"), Title(title).c_str(), player.c_str(), Dimension(dim).c_str(), bpos.c_str());
		wlog(gettime(), title, player, "Open", bpos, "CommandBlock");
	}
}