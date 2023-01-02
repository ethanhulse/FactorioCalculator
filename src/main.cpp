/*
***********************************
    Copyright Ethan M Hulse 2023
***********************************
*/
#include <iostream>
#include <string>
#include <vector>
#include <tuple>


using std::tuple;
using std::get;
using std::make_tuple;
using std::cout;
using std::endl;
using std::string;
using std::vector;


/*
**************************************
        TODO: 1/2/2023
            Fix rate calculations for input ingredients
            Move classes into separate files for readability
            Create an item recipe database. Library for JSON?
            Create UI? Port to Unity?
**************************************
*/

//Want to calculate output rate of crafting recipies based on inputs including assembler levels, assembler numbers, module modifiers 
//Input values are all integers but output values would have to be floating point numbers. 
//Calculate number of assemblers at specified parameters to produce at specified rate? Requires some algebraic reformatting.

//Basic math is OUTPUT_RATE = ITEM_CRAFTING_SPEED * (ASSEMBLER_CRAFTING_SPEED * (NUMBER_OF_SPEED_MODULES * SPEED_MODULE__BONUS%))
//Where all speed modules are the same tier and all assemblers are the same tier. This reduces complexity

//Starter recipies will include their respective crafting times and required materials. 1 gear has a crafting time of 0.5f and a ingredient requirement of 2.0f iron plates. Calculation should at least be agnostic to what the input type is but there needs to
//be separation between multiple inputs. Circuits and iron plates cant be fused into one input.  Vector of inputs and the needed quantity?? vector<Item> {input_a, input_b} then access the vector multiplied by resulting crafting speed to calculate input requirements

//Enum for speed modules and modifiers?
//Class is easier to work with for floats, cant use ints for percentage rates without additional math methods.

class SpeedModule{

    public:
        SpeedModule(){/*Default Constructor*/};
        ~SpeedModule(){/*Default Deconstructor*/};

        SpeedModule(string __name, int __tier)
        {
            this->_tier = __tier;
            this->_name = __name;
            switch(this->_tier)  
            {
                case 1:
                    this->_bonus = 1.2f;
                    break;
                case 2:
                    this->_bonus = 1.3f;
                    break;
                case 3:
                    this->_bonus = 1.5f;
                    break;
                default:
                    cout << "Error in module. Module tier out of bounds. 1-3" << endl;
                    this->_bonus = 1.0f;
                    break;
            }    
        }

        float bonus(){
            return this-> _bonus;
        }

        int tier(){return this->_tier;}
    private:
        string _name;
        int _tier;
        float _bonus;
        //20% 30% 50%
};

//Item class, should include a name/ID, required inputs as vector of Items, base crafting speed.

class Item
{
    public:
        #pragma region Constructors
        //CONSTRUCTORS
        Item()
        {
            /*Default Constructor*/
            this->_name = "N/A";
            this->baseCraftingSpeed = 0;

        }
        ~Item(){/*Default Deconstructor*/}

        Item(string __name, float _baseCraftingSpeed)
        {
            //Constructor without input Items
            this->_name = __name;
            this->baseCraftingSpeed = _baseCraftingSpeed;
        }
        Item(string __name, float _baseCraftingSpeed, vector<tuple<float, Item>> _inputItems)
        {
            //Constructor with input Items
            this->_name = __name;
            this->baseCraftingSpeed = _baseCraftingSpeed;
            this->inputItems = _inputItems;

        }
        #pragma endregion
        #pragma region Getters
        //GETTERS
        float speed()
        {
            return this->baseCraftingSpeed;
        }
        vector<tuple<float, Item>> getInputItems()
        {
            return this->inputItems;
        }
        string name()
        {
            return this->_name;
        }
        #pragma endregion
    private:
        string _name;
        float baseCraftingSpeed;
        //Vector of tuples consisting of an int (Number of Item per cycle) and the prior defined Item.
        vector<tuple<float, Item>> inputItems;
};
tuple<float, Item> TP(float a, Item b)
{
    return make_tuple(a, b);
}
//Assembler Class, similar to speed module. includes a tier id and a crafting speed.
class Assembler
{
    public:
        Assembler()
        {
            this->_tier = 0;
            this->craftingSpeed = 0;
            this->_name = "N/A";
        }
        ~Assembler(){}

        Assembler(string __name, int __tier, int __count)
        {   
            this->_name = __name;
            this->_tier = __tier;
            this->_count = __count;
            switch(_tier)
            {
                case 1:
                    this->craftingSpeed = 0.5f;
                    break;
                case 2:
                    this->craftingSpeed = 0.75f;
                    break;
                case 3:
                    this->craftingSpeed = 1.25f;
                    break;
                default:
                    cout << "Error in Assembler class. Tier is out of bounds. 1-3" << endl;
                    this->craftingSpeed = 0.0f;
                    break;
            }
        }

        float speed(){
            return this->craftingSpeed;
        }
        string name(){
            return this->_name;
        }
        int tier(){
            return this->_tier;
        }
        int count(){
            return this->_count;
        }
    private:
        int _count;
        int _tier;
        float craftingSpeed;
        //0.5, 0.75, 1.25
        string _name;
};

//Handle Output, calculations, operation switching, etc. Pass references of class objects, dont copy whole class.
class Calculation
{
    public:
        Calculation(){};
        ~Calculation(){};


        Calculation(int _opCode,Assembler &__A, SpeedModule &__M, int __moduleCount, Item &__I)
        {

            this->opCode = _opCode;

            this->_A = __A;
            this->_M = __M;
            this->_I = __I;
            this->_moduleCount = __moduleCount;


            //Opcode 1 = calculate for crafted items per second.
            //output switching
            switch(opCode)
            {
                case 1:
                    this->output = this->calculate_crafted_items_per_second(__A, __M, _moduleCount, __I);
                    this->_calculated_outputs = this->calculate_input_item_rate_per_second();
                    break;
                default:
                    this->output = 0.0f;
                    break;
            }
        }



        void PrintOutput()
        {
            cout << "------------------------------------" << endl;
            cout << "Results" << endl;
            cout << "-----------------------" << endl;
            cout << "Assembler Tier: " << _A.tier() << "x" << this-> _A.count() << endl;
            cout << "Module Tier: " <<  _M.tier() << "x" << _moduleCount << endl;
            cout << "Item: " << _I.name() << endl;
            cout << "Result: " << this->output << "/s" << endl;
            cout << "-----------------------" << endl;
            for(int i = 0; i < _calculated_outputs.size(); i++)
            {
                cout << "Input " << i + 1 << ": " << get<1>(_calculated_outputs[i]).name() << endl;
                cout << "   Rate : " << get<0>(_calculated_outputs[i]) << "/s" << endl;
            }
            cout << "------------------------------------" << endl;
        }

        
    private:
        vector<tuple<float, Item>> calculate_input_item_rate_per_second()
        {
            //this->_I.getInputItems()
            vector<tuple<float, Item>> outputVector;
            for(int i = 0; i < this->_input.size(); i++)
            {
                float ff = calculate_crafted_items_per_second(_A, _M, _moduleCount, _I) * get<0>(this->_input[i]);
                cout << "DEBUG: " << get<0>(this->_input[i]);
                outputVector.push_back(TP(((get<0>(this->_input[i]) * ff)), get<1>(this->_input[i])));
            }
            outputVector.shrink_to_fit();
            return outputVector;
        }
        float calculate_crafted_items_per_second(Assembler &A, SpeedModule &M, int Mn, Item &I)
        {
            float As = A.speed();
            float Is = I.speed();
            float Mm = M.bonus();
            float Ac = A.count();
            float AA;
            if(Mn != 0)
                AA = Ac * As * (Mn * Mm);
            else if(Mn == 0)
                AA = Ac * As * Mm;
            float O = Is * AA;
            return O;
        }
        int opCode;
        Assembler _A;
        SpeedModule _M;
        int _moduleCount;
        Item _I;
        int _assemblerCount;
        float output;

        vector<tuple<float, Item>> _input = _I.getInputItems();
        vector<tuple<float, Item>> _calculated_outputs;

};



int main()
{

    //Assembler tier 3
    Assembler assembler_3 = Assembler("assembler_3", 3, 100);

    //Speed module tier 3
    SpeedModule module_3 = SpeedModule("module_3", 3);
    int moduleCount = 0;

    Item iron_plate = Item("iron_plate", 3.2f);
    Item copper_plate = Item("copper_plate", 3.2f);

    vector<tuple<float, Item>> INGREDIENTS_COPPER_WIRE {TP(0.5f, copper_plate)};
    Item copper_wire = Item("copper_wire", 0.5f);
    vector<tuple<float, Item>> INGREDIENTS_GEAR {TP(0.75f, iron_plate)};
    Item gear = Item("gear", 0.5f);
    vector<tuple<float, Item>> INGREDIENTS_GREEN_CIRCUIT{TP(3, copper_wire), TP(1, iron_plate)};
    
    Item green_circuit("green_circuit", 0.5f, INGREDIENTS_GREEN_CIRCUIT);
 





    Calculation newcalc = Calculation(1, assembler_3, module_3, moduleCount, gear);
    newcalc.PrintOutput();

    Calculation calcGreenCircuits = Calculation(1, assembler_3, module_3, moduleCount, green_circuit);
    calcGreenCircuits.PrintOutput();

    //float output = calculate_crafted_items_per_second(assembler_3, module_3, moduleCount, gear);
    //cout << "Calculated Rate for the following inputs" << endl <<"Assembler" << output << endl;
    return 0;

}

