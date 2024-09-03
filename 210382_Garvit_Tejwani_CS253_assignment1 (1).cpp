// Car Rental system
#include <bits/stdc++.h>
using namespace std;
#include <ctime>
class Car{
    int number_plate;
    string model;
    int condition;   // considered condition as a value
    time_t dueDate;
    bool Rented;
    double price;
    int renter_id;  
    int renter_type;
    double due_amount;    
  public:
    // Constructor
    Car(int number_plate, string model , double price) :
        number_plate(number_plate), model(model), condition(0), price(price) , Rented(false), dueDate(0),renter_id(-1),due_amount(0){}
    // getters
    int getId() const {return number_plate;}
    double getdueamount(time_t curr) const{
         curr += 120; // assuming you will clear all dues after 2 minute 
       if(curr >= getDueDate()) return due_amount+(curr-getDueDate())*5;
       else return due_amount;
    }
    string getModel() const { return model;}
    double getprice() const {return price;}
    int getCondition() const { return condition;}
    bool isRented() const{return Rented;}
    time_t getDueDate() const { return dueDate; }
    int getrentertype() const{return renter_type;}
    int getrenterid() const{return renter_id;}

    // Setters
    void setModel(const string& newModel) { model = newModel; }
    void setCondition(const int& newCondition) { condition = newCondition;}
    // Function to rent the car (set as unavailable and update due date)
    void RentCar(time_t rentDate , int id , double initial_amount , int type){       // type 0 -> customer , type 1 -> employee , type 2 -> manager 
        if (!isRented()) {
            Rented = true;
            renter_id = id;
            renter_type = type;
            dueDate = rentDate + 5; // Assuming rent time as 5 secs 
             double deducted_price = price;
            if(type) deducted_price = (0.85)*price;   // 15% discount for employee
            if(deducted_price > initial_amount )  due_amount+= (deducted_price - initial_amount);
            else{
                cout<<"Kindly take your extra money $" << initial_amount-deducted_price << endl;
            }
            cout<<"The Car: "<<endl;
            cout<<"................................."<<endl;
            this->displayInfo(type); 
            cout<<"is rented Sucessfully."<<endl;
        } else {
            cout << "Car ID " <<number_plate<< " is not available for rent." << endl;
        }
    }
    // function to update the due amount of the renter
    void clear_dues(double amount){
        if(!isRented()){
            cout<<"This car is not rented"<<endl;
            return;
        }
         due_amount-=amount;  
    }
    // Function to return the car 
    void returnCar(int new_condition) {
        Rented = false;
        dueDate = 0;
        renter_id = -1;
        condition = new_condition;
        due_amount = 0;
    }
    // Display car details
    void displayInfo(int type ) {
        cout << "Car ID: " << number_plate << endl;
        cout << "Model: " << model << endl;
        cout << "Condition: " << condition << endl;
        if(type == 0 || type == 2 ) cout << "Price: " << price << endl;
        else  cout << "Price: " <<(double)(0.85*price) << endl;
        cout << "Available: " << (Rented ? "No" : "Yes") << endl;
        if (isRented()) {
            cout << "Due Date: " << ctime(&dueDate);
            cout<< "Renter's ID: "<< renter_id << endl;
            cout<< "Renter's type: "<< renter_type << endl;
            cout<< "Due amount : " << getdueamount(time(0)) << endl;
        }
      cout<<".........................."<<endl;
    }
};
map<int,Car*> car_data;
class User{         
   protected:
    int id;
    string name;
    string password;
    int type;          // type -> 0 for customer , type->1 for employee , type -> 2 for Manager
    public:
        // Constructor
        User(const int id, string name, string password , int type ) :
            id(id), name(name), password(password) , type(type) {}
        vector<Car*> rentedCars;
        // Getters
        int getID() const { return id;}
        const string& getName() const { return name;}
        // setters 
        void setname(const string& newname) {name = newname;}
        // getting fine on particular car
        int  get_fine(int carID , time_t curr_time){
            int fine = 0; 
            time_t duedate =  car_data[carID]->getDueDate();
            // cout<<(curr_time)<<" "<<(duedate)<<endl;
            // cout<<(curr_time)<<" "<<(duedate)<<endl;            
            if(curr_time > duedate) fine+=(curr_time-duedate)*5; 
            return fine;
        }
        // clearing all dues of a particular rented car 
        void clear_all_dues(int carID , int amount , int new_condition , time_t curr_time){
            int total_money =  car_data[carID]->getdueamount(curr_time)-120*5; 
            bool ok = false;
            for(auto i : rentedCars){
                if(i->getId() == carID) ok = true;
            }
            if(ok){
            // cout<<"Kindly provide $ "<<total_money<<" for clearing all dues"<<endl;
                if(amount >= total_money){
                    car_data[carID]->returnCar(new_condition);
                    vector<Car*>::iterator it;
                    vector<Car*>::iterator jt;
                    for(jt = rentedCars.begin(); jt!=rentedCars.end();jt++){
                        if((*jt)->getId() == carID){
                            it = jt;
                            break;
                        }

                    }
                    cout<<"Clear_all_dues_Successfully!"<<endl;
                    rentedCars.erase(it);
                }
                else{
                    cout<<"Not sufficient money"<<endl;
                } 
            }
            else{
                cout<<"No car of this Id is rented"<<endl;
            }
        }
        int need_to_pay(int carID , time_t curr){
            return car_data[carID]->getdueamount(curr); // assuming you are paying after 1 minutes so adding also that  
        }
        void clear_due(int carID,int amount){
            if(car_data.find(carID) == car_data.end()){
                cout<<"No car of this id exist"<<endl;
            }
            else car_data[carID]->clear_dues(amount);
        }
        virtual void return_allCar(){} 
        virtual void rentCar(int carID , double initital_amount){}
        // virtual void show_rentedCar(){}
};
class Customer : public User{
    private: 
     int customer_record;
    public:
    // constructor
    Customer(int id, const string& name, const string& password)
    :User(id,name,password,0),customer_record(40){}                           // assuming customer record  40
     // gettters 
    int getcustomer_record() const {return customer_record;}
    // setters
    void setcustomer_record(int& new_record) { customer_record = new_record;} 
      // function for searching and printing available cars
      void searchAvailableCar(){
            for(auto key : car_data){
                Car* c = key.second;
                if(!c->isRented()) c->displayInfo(0);
            }
        }
        // get the total fine value on all currently rented car 
      int get_customer_Totalfine(){
            int fine = 0; 
            time_t curr_time = time(0);
            for(auto c : rentedCars){
                time_t duedate=c->getDueDate();
                if(duedate<curr_time) fine+= (curr_time-duedate)*5; // imposing $5 fine for each second after the due time
            }
            return fine;
        }
        // function for showing currently rented car 
        void show_customer_rentedCar(){
            // cout<<rentedCars.size()<<endl;
             if(!(rentedCars.empty())){ 
                for(int i = 0 ; i < rentedCars.size() ; i++){
                    cout<<"Rented Car: "<<i+1<<": "<<endl;
                    rentedCars[i]->displayInfo(0);
                    cout<<"........................."<<endl;
                }
           }
           else{
               cout<<"No car is rented for now!"<<endl;
           }
        }
        // returning all cars assuming customer is paying whole amount 
      void return_allCar(){
         if(!rentedCars.empty()){
            for(auto c : rentedCars){
                cout<<"Kindly provide Condition of Car having carID: "<<c->getId()<<endl;  
                int new_condition;
                cin>>new_condition;
                c->returnCar(new_condition);
                if(customer_record>=new_condition) customer_record-=new_condition;
                int check_fine = time(0)-(c->getDueDate());
                if(check_fine>0 && !customer_record) customer_record--;
            }
            rentedCars.clear();
            cout<<"You have returned all cars successfully!"<<endl;
         }
         else{
            cout<<"You have no rented Cars"<<endl;
         }
        }
    // add rented cars 
    void rentCar(int carID , double initital_amount){
         if(car_data.find(carID)==car_data.end()){
            //    cout<<"I am here!"<<endl;
                cout<<"No car of this ID found"<<endl;
            }
            else if(customer_record<=0){
                cout<<"Sorry you can't rent this car!"<<endl;
            }
            else{
                // time_t rentDate , int id , double initial_amount , int type
                // cout<<"Kindly give the initial amount to be payed!"<<endl;
                // double initital_amount;
                // cin>>initital_amount;
                car_data[carID]->RentCar(time(0),id,initital_amount,0);
                rentedCars.push_back(car_data[carID]);
                // cout<<"You have rented this car. You have to return it before: "<<car_data[carID]->getDueDate()<<endl;
            }
     }
    // Display customer details (including rented car IDs)
    void return_customer_car(int carID , int new_condition , int amount,time_t curr_time){
        time_t due_time;
        due_time = car_data[carID]->getDueDate();
        if(curr_time > due_time && !customer_record){
            customer_record--;
        }
        if(customer_record> new_condition) customer_record-=(new_condition);        // assuming condition 0 is a new car
        clear_all_dues(carID , amount,new_condition,curr_time);
    }
};
map<int,Customer*> customer_data;
class Employee : public User{
    private: 
     int employee_record;
    public:
    // constructor
    Employee(int id, const string& name, const string& password)
    :User(id,name,password,1),employee_record(40){}                           // assuming customer record  40
     // getters 
    int getemployee_record() const {return employee_record;}
    // setters
    void setemployee_record(int& new_record) { employee_record = new_record;}
     // get the total fine value on all currently rented car 
        int get_employee_Totalfine(){
            int fine = 0; 
            time_t curr_time = time(0);
            for(auto c : rentedCars){
                time_t duedate=c->getDueDate();
                if(duedate<curr_time) fine+= (curr_time-duedate)*5; // imposing $5 fine for each second after the due time
            }
            return fine;
        } 
      // function for searching and printing available cars
      void searchAvailableCar(){
            for(auto key : car_data){
                Car* c = key.second;
                if(!c->isRented()) c->displayInfo(1);
            }
        }
        // function for showing currently rented car 
        void show_employee_rentedCar(){
           if(!rentedCars.empty()){ 
            for(int i = 0 ; i < rentedCars.size() ; i++){
                cout<<"Rented Car "<<i<<": "<<endl;
                rentedCars[i]->displayInfo(1);
                cout<<"........................."<<endl;
            }
           }
           else{
               cout<<"No car is rented for now!"<<endl;
           }
        }
        // returning all cars assuming employee is paying whole amount 
      void return_allCar(){
         if(!rentedCars.empty()){
            for(auto c : rentedCars){
                cout<<"Kindly provide Condition of Car having carID: "<<c->getId()<<endl;  
                int new_condition;
                cin>>new_condition;
                c->returnCar(new_condition);
                if(employee_record>=new_condition) employee_record-=new_condition;
                int check_fine = time(0)-(c->getDueDate());
                if(check_fine>0 && !employee_record) employee_record--;
            }
            rentedCars.clear();
            cout<<"You have returned all cars successfully!"<<endl;
         }
         else{
            cout<<"You have no rented Cars"<<endl;
         }
        }
    // add rented cars 
    void rentCar(int carID , double initital_amount){
         if(car_data.find(carID)==car_data.end()){
                cout<<"No car of this ID found"<<endl;
            }
            else if(car_data[carID]->isRented()){
                cout<<"This car is not available"<<endl;
            }
            else if(employee_record<=0)  cout<<"Sorry you can't rent a car!"<<endl;
            else{ 
                Car* c = car_data[carID];
                c->RentCar(time(0),id,initital_amount,1);
                 rentedCars.push_back(c);
                // cout<<"You have rented this car. You have to return it before: "<<car_data[carID]->getDueDate()<<endl;
            }
     }
    // Display customer details (including rented car IDs)
    void return_employee_car(int carID , int new_condition , int amount , time_t curr_time){
        time_t due_time = car_data[carID]->getDueDate();
        if(curr_time > due_time && !employee_record){
            employee_record--;
        }
        if(employee_record >= new_condition) employee_record-=(new_condition);           // assuming condition 0 is a new car
        clear_all_dues(carID , amount,new_condition,curr_time);
    }
};
map<int,Employee*> employee_data;
class Manager : public User{
    public:
        // constructor 
      Manager(int id, const string& name, const string& password)
       :User(id,name,password,2){} 
    void add_renter(int id ,  string name , string password , int type){
        if(!type){
         if(customer_data.find(id)!=customer_data.end()){
              cout<<"No need to add the customer , it already exist"<<endl;
         }
         else{
         
            Customer* c1 = new Customer(id,name,password);
            customer_data[id] = c1;
            cout<<"Customer added successfully!"<<endl;
         }
        }
        else{
            if(employee_data.find(id)!=employee_data.end()){
                cout<<"No need to add the employee, it already exist"<<endl;
            }
            else{
            Employee* e1 = new Employee(id,name,password);
            employee_data[id] = e1;
            cout<<"Employee added successfully!"<<endl;
            }
        }
    }
    void searchCar(){
          if(!car_data.empty()){ 
            for(auto key : car_data){
                Car* c = key.second;
               c->displayInfo(2);
            }
          }
          else{
            cout<<"No , cars exist"<<endl;
          }
    }
    void searchEmployee(){
       if(!employee_data.empty()){
        for(auto key : employee_data){
            Employee* e = key.second;
            cout<<"Id of employee: "<<e->getID()<<" Name of employee: "<<e->getName()<<" "<<"Record of customer:" <<e->getemployee_record()<<endl;
        }
       }
       else cout<<"No,employee found!"<<endl;
    }
     void searchCustomer(){
        if(!customer_data.empty()){
                for(auto key : customer_data){
                    Customer* e = key.second;
                    cout<<"Id of customer: "<<e->getID()<<" Name of customer: "<<e->getName()<<" "<<"Record of customer:" <<e->getcustomer_record()<<endl;
                }
        }
        else cout<<"No, customer found!"<<endl;
    }
    void delete_renter(int id , int type){
        if(!type){
            if(customer_data.find(id)!=customer_data.end()){
                customer_data[id]->return_allCar();
                customer_data.erase(id);
                cout<<"Customer deleted successfully!"<<endl;
            }
            else{
                cout<<"No customer of this id found!"<<endl;
            }
        }
        else{
            if(employee_data.find(id)!=employee_data.end()){
                employee_data[id]->return_allCar();
                employee_data.erase(id);
                cout<<"employee deleted successfully!"<<endl;
            }
            else{
                cout<<"No employee of this id found!"<<endl;
            }            
        }
    }
    void add_car(int number_plate, string Model, int price){
        if(car_data.find(number_plate)!=car_data.end()){
            cout<<"No need to add the car , it already exist"<<endl;
        }
        else{
         Car* c1 = new Car(number_plate,Model,price);
         car_data[number_plate] = c1;
         cout<<"Car added successfully!"<<endl;
        } 
    }
    void delete_car(int number_plate , time_t curr_time){
        if(car_data.find(number_plate)!=car_data.end()){
            if(car_data[number_plate]->isRented()){
                int type1 = car_data[number_plate]->getrentertype();
                int renters_id = car_data[number_plate]->getrenterid();
                if(!type1){
                    customer_data[renters_id]->clear_all_dues(number_plate,car_data[number_plate]->getdueamount(curr_time),0,curr_time);
                }
                else{
                    employee_data[renters_id]->clear_all_dues(number_plate,car_data[number_plate]->getdueamount(curr_time),0,curr_time);
                }
            }
            car_data.erase(number_plate);
        }
        else cout<<"Car does'nt exit!"<<endl;
    }
    void update_car(bool k , int carID){
        if(!k){
            cout<<"Kindly provide cars new condition!"<<endl;
            int new_condition;
            cin>>new_condition;
            car_data[carID]->setCondition(new_condition);
        }
        else{
             cout<<"Kindly provide car's new Model name!"<<endl;
            string new_Model;
            cin>>new_Model;
            car_data[carID]->setModel(new_Model);
        }
    }
    void update_customer(bool k , int id){
      if(customer_data.find(id)!=customer_data.end()){
        if(!k){
            cout<<"Kindly provide customer's new record!"<<endl;
            int new_record;
            cin>>new_record;
            customer_data[id]->setcustomer_record(new_record);
        }
        else{
             cout<<"Kindly provide customer's new name!"<<endl;
            string new_name;
            cin>>new_name;
            customer_data[id]->setname(new_name);
        }
      }
      else cout<<"No customer of this id exist"<<endl;
    }
    void update_employee(bool k , int id){
       if(employee_data.find(id)!=employee_data.end()){
        if(!k){
            cout<<"Kindly provide employee's new record!"<<endl;
            int new_record;
            cin>>new_record;
            employee_data[id]->setemployee_record(new_record);
        }
        else{
             cout<<"Kindly provide employee's new name!"<<endl;
            string new_name;
            cin>>new_name;
            employee_data[id]->setname(new_name);
        }
       }
       else cout<<"No, employee of this id exist"<<endl;
    }
};
int main()
{    
    Car* car1 = new Car(1234,"NissanVersa",17405);
    Car* car2 = new Car(1243, "Mitsubishi Mirage",18110);
    Car* car3 = new Car(1324 , "Kia Forte" , 20915);
    Car* car4 = new Car(1222, "Hyundai Elantra",22065);
    Car* car5 = new Car(1432 , "Nissan Sentra" , 21295);
    car_data[car1->getId()] = car1;
    car_data[car2->getId()] = car2; 
    car_data[car3->getId()] = car3; 
    car_data[car4->getId()] = car4; 
    car_data[car5->getId()] = car5;
    Customer* c1 = new Customer(210381,"Modi" , "BJP");
    Customer* c2 = new Customer(22222,"chinmay","Gandhi");
    Customer* c3 = new Customer(22233,"Harish","Titu123");
    Customer* c4 = new Customer(22211,"Kishore","GT101");
    Customer* c5 = new Customer(2222111,"Nancy123","Kk11");
    customer_data[c1->getID()] = c1;
    customer_data[c2->getID()] = c2; 
    customer_data[c3->getID()] = c3; 
    customer_data[c4->getID()] = c4; 
    customer_data[c5->getID()] = c5;
    Employee* e1 = new Employee(21032,"AkshayKumar" , "Jaiho!");
    Employee* e2 = new Employee(2222,"SRK","Dabang");
    Employee* e3 = new Employee(2233,"Abhishek","Race");
    Employee* e4 = new Employee(2292,"Ayushmaan","Dunki");
    Employee* e5 = new Employee(22221,"Yash","Salaar");
    employee_data[e1->getID()] = e1;
    employee_data[e2->getID()] = e2; 
    employee_data[e3->getID()] = e3; 
    employee_data[e4->getID()] = e4; 
    employee_data[e5->getID()] = e5;
    Manager* m1 = new Manager(92922,"GarvitTejwani","ghanshyam");
    int x=0;
    // c1->return_customer_car(1234,1,17305,curr);
    while(x!=1){
        cout<<"Welcome to the Car Rental System!";
        cout<<"Type:1 if you are customer|Type:2 if you are employee|Type:3 if you are manager|Type:4 if you want to exit"<<endl;
        int input;
        cin>>input;
        cout<<endl;
        if(input == 1){ 
            cout<<"Enter your id , name and password...";
            int id;
            string name;
            string password;
            cin>>id>>name>>password;
            if(customer_data.find(id)==customer_data.end()){
                cout<<"No customer of this id exist!"<<endl;
            }
           else{ 
            while(true){
                cout<<"Type:1 if you want to know all available cars"<<endl;
                cout<<"Type:2 if you want to see your rented cars lists"<<endl;
                cout<<"Type:3 if you want to rent a car"<<endl;
                cout<<"Type:4 if you want to return a car"<<endl;
                cout<<"Type:5 if you want get your customer_record value"<<endl;
                cout<<"Type:6 if you want to get the total fine on all your rented cars "<<endl;
                cout<<"Type:7 if you want to return your all cars"<<endl;
                cout<<"Type:8 if you want to clear due of a particular car"<<endl;
                cout<<"Type:9 if you want to know what amount you have to pay for a certain car!"<<endl;
                cout<<"Type:10 if you want to exit"<<endl;
                int type;
                cin>>type;
                if(type == 1){
                    customer_data[id]->searchAvailableCar();
                }
                else if(type == 2){
                    customer_data[id]->show_customer_rentedCar();
                }
                else if(type == 3){
                    cout<<"kindly also provide CarID and initital_amount that you are paying for now."<<endl;
                    int carID;
                    double amount ;
                    cin>>carID>>amount;
                    customer_data[id]->rentCar(carID,amount);
                }
                else if(type == 4){
                    cout<<"Kindly provide the carID of the car to be returned , it's new condition value , amount"<<endl;
                    int carID,new_condition;
                    double amount;
                    cin>>carID>>new_condition>>amount;
                    time_t curr = time(0);
                    customer_data[id]->return_customer_car(carID,new_condition,amount,curr);
                }
                else if(type == 5){
                    cout<<customer_data[id]->getcustomer_record()<<endl;
                }
                else if(type == 6){
                    cout<<"Your total fine on all cars is: "<<endl;
                    cout<<customer_data[id]->get_customer_Totalfine()<<endl;
                }
                else if(type == 7){
                    customer_data[id]->return_allCar();
                }
                else if(type == 8){
                    cout<<"Kindly  provide the carID and amount"<<endl;
                    int carID , amount;
                    cin>>carID>>amount;
                    customer_data[id]->clear_due(carID,amount);
                }
                else if(type == 9){
                    cout<<"Kindly provide the Id of that car.";
                    int carID;
                    cin>>carID;
                 cout<<"$"<<customer_data[id]->need_to_pay(carID,time(0))<<"need to be payed"<<endl;
                }
                else if(type == 10){
                    x=1;
                    cout<<"Thanks for visiting this system!"<<endl;
                    break;
                }
            }
         }
        }
        else if(input == 2){
                        cout<<"Enter your id , name and password...";
            int id;
            string name;
            string password;
            cin>>id>>name>>password;
            if(employee_data.find(id)==employee_data.end()){
                cout<<"No employee of this id exist!"<<endl;
            }
           else{ 
            while(true){
                cout<<"Type:1 if you want to know all available cars"<<endl;
                cout<<"Type:2 if you want to see your rented cars lists"<<endl;
                cout<<"Type:3 if you want to rent a car"<<endl;
                cout<<"Type:4 if you want to return a car"<<endl;
                cout<<"Type:5 if you want get your employee_record value"<<endl;
                cout<<"Type:6 if you want to get the total fine on all your rented cars "<<endl;
                cout<<"Type:7 if you want to return your all cars"<<endl;
                cout<<"Type:8 if you want to clear due of a particular car"<<endl;
                cout<<"Type:9 if you want to know what amount you have to pay for a certain car!"<<endl;
                cout<<"Type:10 if you want to exit"<<endl;
                int type;
                cin>>type;
                if(type == 1){
                    employee_data[id]->searchAvailableCar();
                }
                else if(type == 2){
                    employee_data[id]->show_employee_rentedCar();
                }
                else if(type == 3){
                    cout<<"kindly also provide CarID and initital_amount that you are paying for now."<<endl;
                    int carID;
                    double amount ;
                    cin>>carID>>amount;
                    employee_data[id]->rentCar(carID,amount);
                }
                else if(type == 4){
                    cout<<"Kindly provide the carID of the car to be returned , it's new condition value , amount"<<endl;
                    int carID,new_condition;
                    double amount;
                    cin>>carID>>new_condition>>amount;
                    time_t curr = time(0);
                    employee_data[id]->return_employee_car(carID,new_condition,amount,curr);
                }
                else if(type == 5){
                    cout<<employee_data[id]->getemployee_record()<<endl;
                }
                else if(type == 6){
                    cout<<"Your total fine on all cars is: "<<endl;
                    cout<<employee_data[id]->get_employee_Totalfine()<<endl;
                }
                else if(type == 7){
                    employee_data[id]->return_allCar();
                }
                else if(type == 8){
                    cout<<"Kindly  provide the carID and amount"<<endl;
                    int carID , amount;
                    cin>>carID>>amount;
                    employee_data[id]->clear_due(carID,amount);
                }
                else if(type == 9){
                    cout<<"Kindly provide the Id of that car.";
                    int carID;
                    cin>>carID;
                 cout<<"$"<<employee_data[id]->need_to_pay(carID,time(0))<<"need to be payed"<<endl;
                }
                else if(type == 10){
                    x=1;
                    cout<<"Thanks for visiting this system!"<<endl;
                    break;
                }
            }
         }
        }
        else if(input == 3){
            while(true){
                cout<<"Type:1 if you want to add a particular car"<<endl;
                cout<<"Type:2 if you want to delete a particular car"<<endl;
                cout<<"Type:3 if you want to search all cars"<<endl;
                cout<<"Type:4 if you want to add a customer "<<endl;
                cout<<"Type:5 if you want add an employee"<<endl;
                cout<<"Type:6 if you want to delete a customer "<<endl;
                cout<<"Type:7 if you want delete an employee"<<endl;                
                cout<<"Type:8 if you want to set the record of employee"<<endl;
                cout<<"Type:9 if you want to set the record of customer"<<endl;
                cout<<"Type:10 if you want to set the name of customer"<<endl;
                cout<<"Type:11 if you want to set the name of employee"<<endl;
                cout<<"Type:12 if you want to set the condition of a car"<<endl;
                cout<<"Type:13 if you want to set the Model of a car"<<endl;
                cout<<"Type:14 if you want to search the customers"<<endl;
                cout<<"Type:15 if you want to search the employee"<<endl;                
                cout<<"Type:16 if you want to exit"<<endl;
                int type;
                cin>>type;
                if(type == 1){
                    cout<<"Kindly provide the carId , Model and price"<<endl;
                    int carID , price;
                    string Model;
                    cin>>carID>>Model>>price;
                    m1->add_car(carID,Model,price);
                }
                else if(type == 2){
                    cout<<"Kindly provide the carID which is to be deleted"<<endl;
                    int carID;
                    cin>>carID;
                    m1->delete_car(carID,time(0));
                }
                else if(type == 3){
                    m1->searchCar();
                }
                else if(type == 4){
                    cout<<"Kindly provide the customer's Id , name and password"<<endl;
                    int id;
                    string name , password;
                    cin>>id>>name>>password;
                    m1->add_renter(id,name,password,0);
                }
                else if(type ==5 ){
                     cout<<"Kindly provide the employee's Id , name and password"<<endl;
                    int id;
                    string name , password;
                    cin>>id>>name>>password;
                    m1->add_renter(id,name,password,1);
                }
                else if(type == 6){
                    cout<<"Kindly provide the customer's Id"<<endl;
                    int id;
                    cin>>id;
                    m1->delete_renter(id,0);
                }
                else if(type == 7){
                   cout<<"Kindly provide the employee's Id"<<endl;
                    int id;
                    cin>>id;
                    m1->delete_renter(id,1);                    
                }
                else if(type == 8){
                   cout<<"Kindly provide the employee's Id"<<endl;
                    int id ;
                    cin>>id;
                    m1->update_employee(0 , id);
                }
                else if(type == 9){
                     cout<<"Kindly provide the customer's Id"<<endl;
                    int id ;
                    cin>>id;
                    m1->update_customer( 0, id);
                }
                else if(type == 10){
                    cout<<"Kindly provide the customer's Id"<<endl;                    
                    int id ;
                    cin>>id;
                    m1->update_customer( 1, id );
                }
                else if(type == 11){
                    cout<<"Kindly provide the employee's Id"<<endl;                    
                    int id ;
                    cin>>id;
                      m1->update_employee(1,id);
                }
                else if(type == 12){
                    cout<<"Kindly provide the car's Id"<<endl;                    
                    int id;
                    cin>>id;
                    m1->update_car(0,id);
                }
                else if(type == 13){
                    cout<<"Kindly provide the car's Id"<<endl;                    
                      int id;
                      cin>>id;
                      m1->update_car(1,id);
                }
                else if(type == 14){
                    m1->searchCustomer();
                }
                else if(type == 15){
                    m1->searchEmployee();
                }
                else if(type == 16){
                      cout<<"Thanks for visiting this system!"<<endl;
                        x=1;
                     break;
                }
   
          }
        }
        else if(input == 4){
            cout<<"Thanks for visiting this system!"<<endl;
            break;
        }
    }
    // car1->rentCar(time(0),u.getID(),100,0);
    return 0;

}

   