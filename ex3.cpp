/*
try this:
--help --long --long=123 --long 412 --int --int=412 --int 234 --int --plot --float --float=1244
*/
#include <iostream>
#include <string>
#include <regex>
#include <iomanip>
#include <vector>

using namespace std;

struct option {
	const char *name;
	int has_arg;
};

option o[3] = {
	{ "long", 0 },
	{ "int", 1 },
	{ "float", 2 },
};

void launch_help();
void getopt_long(int argc, char **argv);
vector<string> getopt(const string command);

int main(int argc, char **argv)
{
	getopt_long(argc, argv);
	system("pause");
	return 0;
}


vector<string> getopt(const string command) {
	regex  Rx("(^(?!.*=.*$)--(.+)$)|(^(?!.*=$)--((.+)=(.+))$)");
	smatch Match;
	if (!(regex_match(command, Match, Rx)))
		throw runtime_error("Wrong option '" + command + "'");
	if (Match[2] == "help") {
		launch_help();
		return { "" };
	}
	if (Match[2] == "") {
		return { Match[5], Match[6] };
	}
	else {
		return { Match[2] };
	}
}

void launch_help() {
	cout << "|-------------------------------------------------------|" << endl;
	cout << "There is " << size(o) << " options:" << endl;
	for (auto list : o) {
		cout.setf(ios::fixed);
		cout << setw(5) << "--" << list.name;
		switch (list.has_arg)
		{
		case 0:
			cout << " without argument" << endl;
			cout << "        " << "Example: --" << list.name << endl;
			break;
		case 1:
			cout << " requires argument" << endl;
			cout << "        " << "Example: '--" << list.name << "=argument' or '--" << list.name << " argument'" << endl;
			break;
		case 2:
			cout << " with any [argument]" << endl;
			cout << "        " << "Example: '--" << list.name << "' or '--" << list.name << "=argument'" << endl;
			break;
		default:
			break;
		}
	}
	cout << "|-------------------------------------------------------|" << endl << endl;
}

void getopt_long(int argc, char **argv)
{

	for (int i = 1; i < argc; ++i) {
		string command;
		command = argv[i];
		vector<string> res;

		try {
			res = getopt(command);
			bool flag = false;
			for (auto list : o) {
				if (list.name == res[0]) {
					flag = true;
					switch (list.has_arg) {
					case 0:
					{
						cout << "Option '" << res[0] << "' no argumet" << endl;
						break;
					}
					case 1:
					{
						if (res.size() == 2) {
							cout << "Option '" << res[0] << "' with argumet: " << res[1] << endl;
						}
						else {
							if (i == argc - 1) {
								cout << "Wrong option: '" << res[0] << "' (requires argument)" << endl;
							}
							else {
								if (argv[i + 1][0] == '-' && argv[i + 1][1] == '-') {
									cout << "Wrong option: '" << res[0] << "' (requires argument)" << endl;
								}
								else {
									++i;
									cout << "Option: '" << res[0] << "' with argument " << argv[i] << endl;
								}
							}
						}
						break;
					}
					case 2:
					{
						if (res.size() == 2) {
							cout << "Option: '" << res[0] << "' with argumet: [" << res[1] << "]" << endl;
						}
						else {
							cout << "Option: '" << res[0] << "' no argumet" << endl;
						}
						break;
					}
					}
				}
			}
			if (!flag && res[0] != "")
				cout << "Unknown option '" << res[0] << "'\n";
			flag = false;
		}
		catch (exception& ex)
		{
			cout << ex.what() << endl;
		}
	}
}