#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime> 
#include <chrono>
#include <iomanip>
#include "libs/json.hpp"

using json = nlohmann::json;
using namespace std;

struct Task {
    string description;
    bool done;
    string createdOn;
};

string getDate() {
    auto now = chrono::system_clock::now();
    time_t t = chrono::system_clock::to_time_t(now);
    tm* localTime = localtime(&t);
    ostringstream oss;
    oss << put_time(localTime, "%d-%m-%Y");
    return oss.str();
}

vector<Task> tasks;

void saveTasks(const vector<Task>& tasks) {
    json j;
    for (auto& t : tasks) {
        j.push_back({
            {"description", t.description},
            {"done", t.done},
            {"createdOn", t.createdOn}
        });
    }

    ofstream out("tasks.json");
    out << setw(4) << j; // Pretty print JSON
}

vector<Task> loadTasks() {
    vector<Task> tasks;
    ifstream in("tasks.json");
    if (!in) return tasks; // If file doesn't exist yet

    json j;
    in >> j;

    for (auto& item : j) {
        Task t;
        t.description = item.value("description", "");
        t.done = item.value("done", false);
        t.createdOn = item.value("createdOn", getDate());
        tasks.push_back(t);
    }
    return tasks;
}

void listTasks() {
    if (tasks.empty()) {
        cout << "\033[33mNo tasks found.\033[0m\n";
        return;
    }

    cout << "\n\033[36mYour Tasks:\033[0m\n";
    cout << "---------------------------------------------\n";

    for (size_t i = 0; i < tasks.size(); i++) {
        string status = tasks[i].done ? "\033[32m✔ Done\033[0m" : "\033[33m⧗ Pending\033[0m";
        string dateLabel = tasks[i].done ? "Done on " : "Added on ";

        cout << setw(3) << left << i + 1 << ". "
             << "[" << (tasks[i].done ? "\033[32m✔\033[0m" : " ") << "] "
             << setw(25) << left << tasks[i].description.substr(0, 25)
             << " | " << status
             << " | " << dateLabel << tasks[i].createdOn
             << "\n";
    }

    cout << "---------------------------------------------\n";
}

void addTask(const string& desc) {
    string date = getDate();
    tasks.push_back({desc, false, date});
    saveTasks(tasks);
    cout << "Task added: \"" << desc << "\"\n";
}

void addMultipleTasks(const vector<string>& taskList) {
    string date = getDate();
    for (auto& t : taskList)
        tasks.push_back({t, false, date});
    saveTasks(tasks);
    cout << "Added " << taskList.size() << " new tasks.\n";
}

void markDone(int index) {
    if (index < 1 || index > (int)tasks.size()) {
        cout << "Invalid task number.\n";
        return;
    }
    tasks[index - 1].done = true;
    tasks[index - 1].createdOn = getDate();
    saveTasks(tasks);
    cout << "Task " << index << " marked as done.\n";
}

void showHelp() {
    cout << "\n📘 Task Manager CLI - Command Reference\n";
    cout << "----------------------------------------\n";
    cout << "add <task>                     → Add a single task.\n";
    cout << "multi \"task1\" \"task2\" ...  → Add multiple tasks in one go (use quotes around each task).\n";
    cout << "list                           → Display all tasks with their status.\n";
    cout << "done <num1> <num2> ...         → Mark specific tasks as done.\n";
    cout << "done all                       → Mark ALL tasks as done.\n";
    cout << "clear                          → Delete all tasks permanently.\n";
    cout << "help                           → Display this help menu.\n";
    cout << "exit                           → Save and quit.\n";
    cout << "----------------------------------------\n";
    cout << "💡 Example:\n";
    cout << "   tasks add Buy Milk\n";
    cout << "   tasks multi \"Walk Dog\" \"Finish Project\" \"Call Mom\"\n";
    cout << "   tasks done 1 3\n";
    cout << "   tasks done all\n\n";
}

int main() {
    tasks = loadTasks();
    cout << "Welcome back 👑\nType 'help' for commands.\n";

    string command;
    while (true) {
        cout << "\n> ";
        cin >> command;

        if (command == "add") {
            string desc;
            getline(cin, desc);
            if (!desc.empty() && desc[0] == ' ') desc.erase(0, 1);
            if (desc.empty()) cout << "No description entered.\n";
            else addTask(desc);
        }

        else if (command == "multi") {
            vector<string> newTasks;
            string temp;
            getline(cin, temp);
            stringstream ss(temp);
            string word;
            while (ss >> quoted(word)) {
                newTasks.push_back(word);
            }
            if (newTasks.empty())
                cout << "No tasks entered.\n";
            else
                addMultipleTasks(newTasks);
        }

        else if (command == "done") {
            string line;
            getline(cin, line);
            if (!line.empty() && line[0] == ' ')
                line.erase(0, 1);

            if (line == "all") {
                for (auto& t : tasks) {
                    t.done = true;
                    t.createdOn = getDate();
                }
                saveTasks(tasks);
                cout << "All tasks marked as done ✅\n";
            } else {
                stringstream ss(line);
                int idx;
                bool any = false;
                while (ss >> idx) {
                    if (idx > 0 && idx <= (int)tasks.size()) {
                        tasks[idx - 1].done = true;
                        tasks[idx - 1].createdOn = getDate();
                        any = true;
                    }
                }
                if (any) {
                    saveTasks(tasks);
                    cout << "Selected tasks marked as done ✅\n";
                } else {
                    cout << "No valid task numbers entered 😅\n";
                }
            }
        }

        else if (command == "list") listTasks();

        else if (command == "clear") {
            cout << "⚠️  Are you sure you want to delete all tasks? (y/n): ";
            char choice;
            cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                ofstream out("tasks.json", ios::trunc);
                out.close();
                tasks.clear();
                cout << "All tasks deleted. 💀\n";
            } else {
                cout << "Operation cancelled. 😌\n";
            }
        }

        else if (command == "help") showHelp();

        else if (command == "exit") {
            saveTasks(tasks);
            cout << "Farewell 👋\n";
            break;
        }

        else {
            cout << "Unknown command. Type 'help' if you’ve forgotten commands.\n";
        }
    }

    return 0;
}
