#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <map>  

class Box {
private:
    int boxNumber;
    int nominal;
    int count;

public:
    Box(int number) {
        boxNumber = number;
        nominal = 0;
        count = 0;
    }

    int getBoxNumber() const { return boxNumber; }
    int getNominal() const { return nominal; }
    int getCount() const { return count; }
    int getTotalAmount() const { return nominal * count; }

    void load(int newNominal, int newCount) {
        if (newCount < 0 || newCount > 100) {
            throw std::runtime_error("Invalid number of banknotes. Max 100 per box.");
        }
        if (newNominal != 10 && newNominal != 20 &&
            newNominal != 50 && newNominal != 100) {
            throw std::runtime_error("Invalid nominal value.");
        }
        nominal = newNominal;
        count = newCount;
    }

    int withdraw(int needed, int& usedCount) {
        if (nominal == 0 || count == 0 || needed < nominal) {
            usedCount = 0;
            return 0;
        }

        usedCount = std::min(count, needed / nominal);
        int taken = usedCount * nominal;
        count -= usedCount;
        return taken;
    }

    void printStatus() const {
        std::cout << "Box #" << boxNumber
            << " | Nominal: " << nominal << " Euro"
            << " | Number of banknotes: " << count
            << " | Total: " << getTotalAmount() << " Euro\n";
    }
};

class ATM {
private:
    std::vector<Box> boxes;

public:
    ATM() {
        for (int i = 1; i <= 6; ++i) {
            boxes.emplace_back(i);
        }
    }

    void processCommand(const std::string& command) {
        if (command.empty()) return;

        if (command[0] == 'L') {
            loadBox(command);
        }
        else if (command[0] == 'W') {
            withdrawMoney(command);
        }
        else if (command[0] == 'P') {
            printStatus();
        }
        else {
            throw std::runtime_error("Unknown command.");
        }
    }

private:
    void loadBox(const std::string& cmd) {
        int boxNo, nominal, count;
        if (parseLoad(cmd, boxNo, nominal, count)) {
            if (boxNo < 1 || boxNo > 6)
                throw std::runtime_error("Invalid box number.");
            boxes[boxNo - 1].load(nominal, count);
        }
    }

    void withdrawMoney(const std::string& cmd) {
        int amount;
        if (!parseWithdraw(cmd, amount))
            throw std::runtime_error("Invalid withdrawal command.");

        if (amount > 3000)
            throw std::runtime_error("Amount exceeds 3000 EUR.");

        std::vector<std::tuple<int, int, int>> withdrawalResult;

        std::vector<Box*> sortedBoxes;
        for (Box& b : boxes)
            sortedBoxes.push_back(&b);
        std::sort(sortedBoxes.begin(), sortedBoxes.end(), [](Box* a, Box* b) {
            return a->getNominal() > b->getNominal();
            });

        // preview step (no real withdrawal yet)
        std::vector<std::tuple<int, int, int>> preview;
        int previewRemaining = amount;
        for (Box* box : sortedBoxes) {
            int nominal = box->getNominal();
            if (nominal == 0) continue;

            int available = box->getCount();
            int canUse = std::min(available, previewRemaining / nominal);

            if (canUse > 0) {
                preview.emplace_back(box->getBoxNumber(), nominal, canUse);
                previewRemaining -= canUse * nominal;
            }

            if (previewRemaining == 0) break;
        }

        if (previewRemaining > 0)
            throw std::runtime_error("Insufficient funds or denominations to fulfill the request.");

        for (const auto& t : preview) {
            int boxNo = std::get<0>(t);
            int nominal = std::get<1>(t);
            int used = std::get<2>(t);

            Box& box = boxes[boxNo - 1];
            int dummy;
            box.withdraw(nominal * used, dummy);
            withdrawalResult.emplace_back(boxNo, nominal, used);
        }

        std::cout << "\n Withdrawal Successful:\n";
        for (const auto& item : withdrawalResult) {
            int boxNo = std::get<0>(item);
            int nom = std::get<1>(item);
            int used = std::get<2>(item);
            int subtotal = nom * used;
            std::cout << "  Box #" << boxNo
                << " to " << used << " x " << nom << " Euro"
                << " equal " << subtotal << " Euro\n";
        }
        std::cout << std::endl;
    }

    void printStatus() {
        int total = 0;
        std::cout << "\n Current ATM Status:\n";
        for (const Box& box : boxes) {
            box.printStatus();
            total += box.getTotalAmount();
        }

		std::cout << "-------------------------------------\n";
        std::cout << " TOTAL CASH AVAILABLE: " << total << "Euro\n\n";
    }

    bool parseLoad(const std::string& cmd, int& boxNo, int& nominal, int& count) {
        char dummy;
        std::istringstream ss(cmd);
        if (ss >> dummy >> dummy >> boxNo >> dummy >> nominal >> dummy >> count)
            return true;
        return false;
    }

    bool parseWithdraw(const std::string& cmd, int& amount) {
        char dummy;
        std::istringstream ss(cmd);
        if (ss >> dummy >> dummy >> amount)
            return true;
        return false;
    }
};

int main()
{
    ATM atm;
    std::string input;

    std::cout << " ATM Console Ready\n";
    std::cout << "Use commands:\n";
    std::cout << "  L#<box>#<nominal>#<count>  Load banknotes\n";
    std::cout << "  W#<amount>                 Withdraw money\n";
    std::cout << "  P                          Print ATM status\n";
    std::cout << "Press Ctrl+C to exit.\n\n";

    while (std::getline(std::cin, input)) {
        try {
            atm.processCommand(input);
        }
        catch (const std::exception& e) {
            std::cerr << " Error: " << e.what() << "\n\n";
        }
    }

    return 0;
}
