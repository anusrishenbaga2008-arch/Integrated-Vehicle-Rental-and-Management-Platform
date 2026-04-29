#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <list>
#include <algorithm>
#include <stdexcept>

using namespace std;

// ================= USER =================
struct User {
    string username;
    string password;
    string role;
};

vector<User> users;

// ================= NAMESPACE =================
namespace Utility {
    void greet() {
        cout << "\nWelcome to Vehicle Rental System\n";
    }
}

// ================= TEMPLATE =================
template <typename T>
T calculateTotal(T a, T b) {
    return a + b;
}

template <class T>
class Logger {
public:
    void log(T msg) {
        cout << "LOG: " << msg << endl;
    }
};

// ================= STRING HANDLING =================
string toUpperCase(string s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

string trim(string s) {
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    return s;
}

// ================= COST =================
double calcCost(int days, double price) {
    return days * price;
}

double calcCost(int days, double price, double discount) {
    return (days * price) - discount;
}

// ================= UI =================
void showTitle() {
    cout << "\n============================================================\n";
    cout << "   INTEGRATED VEHICLE RENTAL AND MANAGEMENT PLATFORM\n";
    cout << "============================================================\n";
}

void boxLine() {
    cout << "+-----+------------+----------+----------+----------+------------+----------+\n";
}

void header() {
    boxLine();
    cout << "| " << left << setw(3) << "ID"
         << "| " << setw(10) << "Name"
         << "| " << setw(8) << "Type"
         << "| " << setw(8) << "Fuel"
         << "| " << setw(8) << "Price"
         << "| " << setw(10) << "Status"
         << "| " << setw(8) << "Extra" << "|\n";
    boxLine();
}

// ================= VEHICLE =================
class Vehicle {
protected:
    int id;
    string name, fuel, status;
    double price;

public:
    Vehicle(int i, string n, string f, double p)
        : id(i), name(n), fuel(f), price(p), status("Available") {}

    virtual void display() {
        cout << "| " << left << setw(3) << id
             << "| " << setw(10) << name
             << "| " << setw(8) << getType()
             << "| " << setw(8) << fuel
             << "| " << setw(8) << price
             << "| " << setw(10) << status;
    }

    virtual string getType() = 0;

    int getID() { return id; }
    string getStatus() { return status; }
    double getPrice() { return price; }

    void setStatus(string s) { status = s; }
    void setPrice(double p) { price = p; }

    virtual ~Vehicle() {}
};

class Car : public Vehicle {
    int doors;
public:
    Car(int id, string n, string f, double p, int d)
        : Vehicle(id, n, f, p), doors(d) {}

    void display() override {
        Vehicle::display();
        cout << "| " << setw(8) << doors << "|\n";
        boxLine();
    }

    string getType() override { return "Car"; }
};

class Bike : public Vehicle {
    int cc;
public:
    Bike(int id, string n, string f, double p, int c)
        : Vehicle(id, n, f, p), cc(c) {}

    void display() override {
        Vehicle::display();
        cout << "| " << setw(8) << cc << "|\n";
        boxLine();
    }

    string getType() override { return "Bike"; }
};

class Truck : public Vehicle {
    double payload;
public:
    Truck(int id, string n, string f, double p, double pl)
        : Vehicle(id, n, f, p), payload(pl) {}

    void display() override {
        Vehicle::display();
        cout << "| " << setw(8) << payload << "|\n";
        boxLine();
    }

    string getType() override { return "Truck"; }
};

// ================= GLOBAL =================
vector<Vehicle*> vehicles;
list<string> bookingHistory;
list<string> maintenanceLog;
int vehicleCounter = 1;
double totalRevenue = 0;

// ================= FILE HANDLING =================
void saveUsers() {
    ofstream f("users.txt");
    for (auto &u : users)
        f << u.username << " " << u.password << " " << u.role << endl;
}

void loadUsers() {
    ifstream f("users.txt");
    User u;
    while (f >> u.username >> u.password >> u.role)
        users.push_back(u);
}

void saveBookings() {
    ofstream f("bookings.txt");
    for (auto &b : bookingHistory)
        f << b << endl;
}

void loadBookings() {
    ifstream f("bookings.txt");
    string line;
    while (getline(f, line))
        bookingHistory.push_back(line);
}

void saveMaintenance() {
    ofstream f("maintenance.txt");
    for (auto &m : maintenanceLog)
        f << m << endl;
}

void loadMaintenance() {
    ifstream f("maintenance.txt");
    string line;
    while (getline(f, line))
        maintenanceLog.push_back(line);
}

void saveVehicles() {
    ofstream f("vehicles.txt");
    for (auto v : vehicles)
        f << v->getID() << " "
          << v->getType() << " "
          << v->getPrice() << " "
          << v->getStatus() << endl;
}

void loadVehicles() {
    ifstream f("vehicles.txt");
    int id; string type, status; double price;

    while (f >> id >> type >> price >> status) {
        if (type == "Car")
            vehicles.push_back(new Car(id, "KIA", "Petrol", price, 4));
        else if (type == "Bike")
            vehicles.push_back(new Bike(id, "BMW", "Petrol", price, 150));
        else if(type=="Truck")
            vehicles.push_back(new Truck(id, "Honda", "Diesel", price, 500));

        vehicles.back()->setStatus(status);
        vehicleCounter = max(vehicleCounter, id + 1);
    }
}

// ================= AUTH =================
void registerUser(string role) {
    User u;
    cout << "Username: ";
    getline(cin >> ws, u.username);
    cout << "Password: ";
    getline(cin >> ws, u.password);
    u.role = role;
    users.push_back(u);
    cout << " Registered!\n";
}

bool login(string role) {
    string u, p;
    cout << "Username: ";
    getline(cin >> ws, u);
    cout << "Password: ";
    getline(cin >> ws, p);

    u = trim(u);
    p = trim(p);

    for (auto &usr : users)
        if (usr.username == u && usr.password == p && usr.role == role)
            return true;

    cout << " Invalid login\n";
    return false;
}

// ================= PAYMENT =================
class Payment {
public:
    void pay(double amt) {
        int m;
        cout << "1.Card 2.UPI 3.Cash: ";
        cin >> m;
        if (m < 1 || m > 3) throw runtime_error("Invalid payment");
        cout << "Paid: " << amt << endl;
    }
};

// ================= ADMIN =================
class Admin {
public:
    void addVehicle() {
        string type, name, fuel;
        double price;

        cout << "Enter Type: ";
        getline(cin >> ws, type);
        cout << "Enter Name: ";
        getline(cin >> ws, name);
        cout << "Enter Fuel: ";
        getline(cin >> ws, fuel);
        cout << "Enter Price: ";
        cin >> price;

        type = toUpperCase(type);

        if (type == "CAR") {
            int d; cout<<"Doors: "; cin>>d;
            vehicles.push_back(new Car(vehicleCounter++, name, fuel, price, d));
        } else if (type == "BIKE") {
            int c; cout<<"CC: "; cin>>c;
            vehicles.push_back(new Bike(vehicleCounter++, name, fuel, price, c));
        } else {
            double p; cout<<"Payload: "; cin>>p;
            vehicles.push_back(new Truck(vehicleCounter++, name, fuel, price, p));
        }
    }

    void updateVehicle() {
        int id; double p;
        cout << "Enter ID and new price: ";
        cin >> id >> p;
        for (auto v : vehicles)
            if (v->getID() == id) v->setPrice(p);
    }

    void managePricing() {
        double percent;
        cout << "Enter % increase: ";
        cin >> percent;
        for (auto v : vehicles)
            v->setPrice(v->getPrice() + v->getPrice()*percent/100);
    }

    void retireVehicle() {
        int id;
        cout << "Enter Vehicle ID: ";
        cin >> id;
        for (auto v : vehicles)
            if (v->getID()==id) v->setStatus("Retired");
    }

    void manageUsers() {
        cout << "\n--- Users ---\n";
        for (auto &u : users)
            cout << u.username << " (" << u.role << ")\n";
    }

    void generateReports() {
        cout << "\n===== REPORT =====\n";
        cout << "Total Revenue: " << totalRevenue << endl;

        cout << "\nBookings:\n";
        for (auto &b : bookingHistory)
            cout << b << endl;

        cout << "\nMaintenance Logs:\n";
        for (auto &m : maintenanceLog)
            cout << m << endl;
    }

    void view() {
        header();
        for (auto v : vehicles) v->display();
    }
};

// ================= CUSTOMER =================
class Customer {
public:
    void view() {
        header();
        for (auto v : vehicles)
            if (v->getStatus()=="Available")
                v->display();
    }

    void search() {
        string type;
        cout << "Enter Type: ";
        getline(cin >> ws, type);
        type = toUpperCase(type);

        header();
        for (auto v : vehicles)
            if (toUpperCase(v->getType())==type && v->getStatus()=="Available")
                v->display();
    }

    void book(Payment &p) {
        int id,d;
        cout << "Enter ID and Days: ";
        cin >> id >> d;
        for (auto v:vehicles) {
            if (v->getID()==id && v->getStatus()=="Available") {
                double cost = calcCost(d,v->getPrice(),50);
                p.pay(cost);
                v->setStatus("Booked");
                totalRevenue += cost;
                bookingHistory.push_back("Booked ID "+to_string(id));
                return;
            }
        }
    }

    void manageBookings() {
        int ch,id;
        cout << "1.Cancel 2.Extend\nChoice: ";
        cin >> ch;
        cout << "Enter Vehicle ID: ";
        cin >> id;

        for (auto v:vehicles) {
            if (v->getID()==id) {
                if (ch==1) {
                    v->setStatus("Available");
                    cout << "Cancelled\n";
                }
                else {
                    int d;
                    cout << "Extra days: ";
                    cin >> d;
                    double extra = calcCost(d,v->getPrice());
                    totalRevenue += extra;
                    cout << "Extended\n";
                }
            }
        }
    }

    void manageProfile() {
        string uname, pass;
        cout << "Enter new username: ";
        getline(cin >> ws, uname);
        cout << "Enter new password: ";
        getline(cin >> ws, pass);

        for (auto &u : users) {
            if (u.role == "customer") {
                u.username = uname;
                u.password = pass;
                cout << " Profile Updated\n";
                return;
            }
        }
    }
};

// ================= AGENT =================
class Agent {
public:

    void viewBookings() {
        header();
        for (auto v : vehicles)
            if (v->getStatus() == "Booked" || v->getStatus() == "Rented" || v->getStatus() == "Returned")
                v->display();
    }

    void pickup() {
        int id;
        cout << "Enter ID: ";
        cin >> id;

        for (auto v : vehicles) {
            if (v->getID() == id && v->getStatus() == "Booked") {
                v->setStatus("Rented");
                cout << "Vehicle picked up\n";
                return;
            }
        }

        cout << " Invalid ID or Vehicle not in Booked state\n";
    }

    void ret() {
        int id;
        cout << "Enter ID: ";
        cin >> id;

        for (auto v : vehicles) {
            if (v->getID() == id && v->getStatus() == "Rented") {
                v->setStatus("Returned");
                cout << " Vehicle returned. Awaiting inspection\n";
                return;
            }
        }

        cout << " Invalid ID or Vehicle not in Rented state\n";
    }

    void inspectVehicle() {
        int id, cond;
        cout << "Enter ID and Condition (1=Good, 0=Damaged): ";
        cin >> id >> cond;

        for (auto v : vehicles) {
            if (v->getID() == id && v->getStatus() == "Returned") {

                if (cond == 1) {
                    v->setStatus("Available");
                    cout << " Vehicle OK -- Available\n";
                } else {
                    v->setStatus("Maintenance");
                    maintenanceLog.push_back("Vehicle " + to_string(id) + " damaged");
                    cout << "Vehicle damaged -- Sent to Maintenance\n";
                }
                return;
            }
        }

        cout << " Invalid ID or Vehicle not ready for inspection\n";
    }
};

// ================= MECHANIC =================
class Mechanic {
public:

    void viewSchedule() {
        cout << "\n--- Maintenance Schedule ---\n";
        for (auto &m : maintenanceLog)
            cout << m << endl;
    }

    void record() {
        int id;
        cout << "Enter Vehicle ID for servicing: ";
        cin >> id;

        for (auto v : vehicles) {
            if (v->getID() == id && v->getStatus() == "Maintenance") {
                maintenanceLog.push_back("Vehicle " + to_string(id) + " serviced");
                cout << " Service recorded\n";
                return;
            }
        }

        cout << " Vehicle not in Maintenance state\n";
    }

    void fix() {
        int id;
        cout << "Enter Vehicle ID to fix: ";
        cin >> id;

        for (auto v : vehicles) {
            if (v->getID() == id && v->getStatus() == "Maintenance") {
                v->setStatus("Available");
                maintenanceLog.push_back("Vehicle " + to_string(id) + " fixed and returned");
                cout << " Vehicle repaired → Available\n";
                return;
            }
        }

        cout << " Invalid ID or Vehicle not under maintenance\n";
    }
};
// ================= MAIN =================
int main() {

    // Default users added
    users.push_back({"admin","123","admin"});
    users.push_back({"agent","123","agent"});
    users.push_back({"mechanic","123","mechanic"});

    Utility::greet();
    showTitle();

    Logger<string> log;
    log.log("System Started");

    loadUsers();
    loadVehicles();
    loadBookings();
    loadMaintenance();

    Admin admin;
    Customer customer;
    Agent agent;
    Mechanic mech;
    Payment pay;

    int role, op;

    do {
        cout << "\n========= MAIN MENU =========\n";
        cout << "1. Admin Login \n2. Register Customer\n3. Customer Login\n4. Agent Login\n5. Mechanic Login\n0. Exit\n";
        cout<<"Enter Choice:";
        cin >> role;

        if (role==1 && login("admin")) {
            do {
                cout << "\n1.Add 2.Update 3.Pricing 4.View 5.Retire 6.Users 7.Report 0.Logout\n";
                cin >> op;

                if(op==1) admin.addVehicle();
                else if(op==2) admin.updateVehicle();
                else if(op==3) admin.managePricing();
                else if(op==4) admin.view();
                else if(op==5) admin.retireVehicle();
                else if(op==6) admin.manageUsers();
                else if(op==7) admin.generateReports();

            } while(op!=0);
        }

        else if(role==2) registerUser("customer");

        else if(role==3 && login("customer")) {
            do {
                cout << "\n1.View 2.Search 3.Book 4.Manage Booking 5.Profile 0.Logout\n";
                cin >> op;

                if(op==1) customer.view();
                else if(op==2) customer.search();
                else if(op==3) customer.book(pay);
                else if(op==4) customer.manageBookings();
                else if(op==5) customer.manageProfile();

            } while(op!=0);
        }

        else if(role==4 && login("agent")) {
            do {
                cout << "\n1.View Bookings 2.Pickup 3.Return 4.Inspect 0.Logout\n";
                cin >> op;

                if(op==1) agent.viewBookings();
                else if(op==2) agent.pickup();
                else if(op==3) agent.ret();
                else if(op==4) agent.inspectVehicle();

            } while(op!=0);
        }

        else if(role==5 && login("mechanic")) {
            do {
                cout << "\n1.View Schedule 2.Record 3.Fix 0.Logout\n";
                cin >> op;

                if(op==1) mech.viewSchedule();
                else if(op==2) mech.record();
                else if(op==3) mech.fix();

            } while(op!=0);
        }

    } while(role!=0);

    saveUsers();
    saveVehicles();
    saveBookings();
    saveMaintenance();

    for (auto v:vehicles) delete v;

    cout << "\nThank you for using the system\n";
}
