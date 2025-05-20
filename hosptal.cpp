#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

// ===================== Person Base Class =====================
class Person {
protected:
    string name;
    int age;
    char gender;

public:
    void inputPersonDetails() {
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Age: ";
        cin >> age;
        cout << "Enter Gender (M/F): ";
        cin >> gender;
        cin.ignore();
    }

    void displayPersonDetails() const {
        cout << "Name: " << name << ", Age: " << age << ", Gender: " << gender << endl;
    }
};

// ===================== Patient =====================
class Patient : public Person {
private:
    string patientID;
    string disease;

public:
    void inputPatient() {
        cout << "Enter Patient ID: ";
        getline(cin, patientID);
        inputPersonDetails();
        cout << "Enter Disease: ";
        getline(cin, disease);
    }

    void displayPatient() const {
        cout << "\n--- Patient Info ---\n";
        cout << "ID: " << patientID << endl;
        displayPersonDetails();
        cout << "Disease: " << disease << endl;
    }

    string getPatientID() const { return patientID; }

    string serialize() const {
        return patientID + "|" + name + "|" + to_string(age) + "|" + gender + "|" + disease;
    }

    void deserialize(const string& line) {
        stringstream ss(line);
        string ageStr;
        getline(ss, patientID, '|');
        getline(ss, name, '|');
        getline(ss, ageStr, '|');
        age = stoi(ageStr);
        ss >> gender;
        ss.ignore();
        getline(ss, disease);
    }
};

// ===================== Doctor =====================
class Doctor : public Person {
private:
    string doctorID;
    string specialization;

public:
    void inputDoctor() {
        cout << "Enter Doctor ID: ";
        getline(cin, doctorID);
        inputPersonDetails();
        cout << "Enter Specialization: ";
        getline(cin, specialization);
    }

    void displayDoctor() const {
        cout << "\n--- Doctor Info ---\n";
        cout << "ID: " << doctorID << endl;
        displayPersonDetails();
        cout << "Specialization: " << specialization << endl;
    }

    string getDoctorID() const { return doctorID; }

    string serialize() const {
        return doctorID + "|" + name + "|" + to_string(age) + "|" + gender + "|" + specialization;
    }

    void deserialize(const string& line) {
        stringstream ss(line);
        string ageStr;
        getline(ss, doctorID, '|');
        getline(ss, name, '|');
        getline(ss, ageStr, '|');
        age = stoi(ageStr);
        ss >> gender;
        ss.ignore();
        getline(ss, specialization);
    }
};

// ===================== Appointment =====================
class Appointment {
private:
    string patientID, doctorID, date;

public:
    void inputAppointment() {
        cout << "Enter Patient ID: ";
        getline(cin, patientID);
        cout << "Enter Doctor ID: ";
        getline(cin, doctorID);
        cout << "Enter Date (YYYY-MM-DD): ";
        getline(cin, date);
    }

    void displayAppointment() const {
        cout << "Appointment: Patient ID = " << patientID
             << ", Doctor ID = " << doctorID
             << ", Date = " << date << endl;
    }

    string serialize() const {
        return patientID + "|" + doctorID + "|" + date;
    }

    void deserialize(const string& line) {
        stringstream ss(line);
        getline(ss, patientID, '|');
        getline(ss, doctorID, '|');
        getline(ss, date);
    }
};

// ===================== Bill =====================
class Bill {
private:
    string patientID, date;
    float consultationFee, testCharges, medicationCharges;

public:
    void inputBill() {
        cout << "Enter Patient ID: ";
        getline(cin, patientID);
        cout << "Enter Date (YYYY-MM-DD): ";
        getline(cin, date);
        cout << "Consultation Fee: ";
        cin >> consultationFee;
        cout << "Test Charges: ";
        cin >> testCharges;
        cout << "Medication Charges: ";
        cin >> medicationCharges;
        cin.ignore();
    }

    void displayBill() const {
        float total = consultationFee + testCharges + medicationCharges;
        cout << "\n--- Bill ---\n";
        cout << "Patient ID: " << patientID << "\nDate: " << date << endl;
        cout << "Consultation: $" << consultationFee << ", Tests: $" << testCharges
             << ", Medications: $" << medicationCharges << endl;
        cout << "Total: $" << total << endl;
    }

    string getPatientID() const { return patientID; }

    string serialize() const {
        return patientID + "|" + date + "|" + to_string(consultationFee) + "|" +
               to_string(testCharges) + "|" + to_string(medicationCharges);
    }

    void deserialize(const string& line) {
        stringstream ss(line);
        string cf, tc, mc;
        getline(ss, patientID, '|');
        getline(ss, date, '|');
        getline(ss, cf, '|');
        getline(ss, tc, '|');
        getline(ss, mc);
        consultationFee = stof(cf);
        testCharges = stof(tc);
        medicationCharges = stof(mc);
    }
};

// ===================== Hospital =====================
class Hospital {
private:
    vector<Patient> patients;
    vector<Doctor> doctors;
    vector<Appointment> appointments;
    vector<Bill> bills;

public:
    Hospital() {
        loadPatients();
        loadDoctors();
        loadAppointments();
        loadBills();
    }

    // File Loaders
    void loadPatients() {
        ifstream in("patients.txt");
        string line;
        while (getline(in, line)) {
            Patient p;
            p.deserialize(line);
            patients.push_back(p);
        }
    }

    void loadDoctors() {
        ifstream in("doctors.txt");
        string line;
        while (getline(in, line)) {
            Doctor d;
            d.deserialize(line);
            doctors.push_back(d);
        }
    }

    void loadAppointments() {
        ifstream in("appointments.txt");
        string line;
        while (getline(in, line)) {
            Appointment a;
            a.deserialize(line);
            appointments.push_back(a);
        }
    }

    void loadBills() {
        ifstream in("bills.txt");
        string line;
        while (getline(in, line)) {
            Bill b;
            b.deserialize(line);
            bills.push_back(b);
        }
    }

    // Save functions
    void saveToFile(const string& filename, const string& data) {
        ofstream out(filename, ios::app);
        out << data << "\n";
    }

    // Patients
    void addPatient() {
        Patient p;
        p.inputPatient();
        patients.push_back(p);
        saveToFile("patients.txt", p.serialize());
        cout << "✅ Patient added.\n";
    }

    void showPatients() const {
        for (const auto& p : patients)
            p.displayPatient();
    }

    void findPatient() const {
        string id;
        cout << "Enter Patient ID: ";
        getline(cin, id);
        for (const auto& p : patients)
            if (p.getPatientID() == id) return p.displayPatient();
        cout << "❌ Patient not found.\n";
    }

    // Doctors
    void addDoctor() {
        Doctor d;
        d.inputDoctor();
        doctors.push_back(d);
        saveToFile("doctors.txt", d.serialize());
        cout << "✅ Doctor added.\n";
    }

    void showDoctors() const {
        for (const auto& d : doctors)
            d.displayDoctor();
    }

    void findDoctor() const {
        string id;
        cout << "Enter Doctor ID: ";
        getline(cin, id);
        for (const auto& d : doctors)
            if (d.getDoctorID() == id) return d.displayDoctor();
        cout << "❌ Doctor not found.\n";
    }

    // Appointments
    void scheduleAppointment() {
        Appointment a;
        a.inputAppointment();
        appointments.push_back(a);
        saveToFile("appointments.txt", a.serialize());
        cout << "✅ Appointment scheduled.\n";
    }

    void showAppointments() const {
        for (const auto& a : appointments)
            a.displayAppointment();
    }

    // Billing
    void generateBill() {
        Bill b;
        b.inputBill();
        bills.push_back(b);
        saveToFile("bills.txt", b.serialize());
        cout << "✅ Bill generated.\n";
    }

    void viewBillsByPatient() const {
        string id;
        cout << "Enter Patient ID: ";
        getline(cin, id);
        bool found = false;
        for (const auto& b : bills) {
            if (b.getPatientID() == id) {
                b.displayBill();
                found = true;
            }
        }
        if (!found)
            cout << "❌ No bills found for this patient.\n";
    }
};

// ===================== Admin Login =====================
bool login() {
    string username, password;
    cout << "===== Admin Login =====\n";
    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, password);
    return (username == "admin" && password == "admin123");
}

// ===================== Main Function =====================
int main() {
    if (!login()) {
        cout << "❌ Login failed. Exiting...\n";
        return 0;
    }

    Hospital hospital;
    int choice;

    do {
        cout << "\n======= Hospital Management System =======\n";
        cout << "1. Add Patient\n";
        cout << "2. View All Patients\n";
        cout << "3. Find Patient by ID\n";
        cout << "4. Add Doctor\n";
        cout << "5. View All Doctors\n";
        cout << "6. Find Doctor by ID\n";
        cout << "7. Schedule Appointment\n";
        cout << "8. View Appointments\n";
        cout << "9. Generate Bill\n";
        cout << "10. View Bills by Patient ID\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: hospital.addPatient(); break;
            case 2: hospital.showPatients(); break;
            case 3: hospital.findPatient(); break;
            case 4: hospital.addDoctor(); break;
            case 5: hospital.showDoctors(); break;
            case 6: hospital.findDoctor(); break;
            case 7: hospital.scheduleAppointment(); break;
            case 8: hospital.showAppointments(); break;
            case 9: hospital.generateBill(); break;
            case 10: hospital.viewBillsByPatient(); break;
            case 0: cout << "👋 Exiting... Goodbye!\n"; break;
            default: cout << "❗ Invalid option. Try again.\n"; break;
        }
    } while (choice != 0);

    return 0;
}

       
    