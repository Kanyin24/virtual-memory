#include <iostream>
#include "header_hash.h"
using namespace std;

int main()
{
    Hash_table* hashtab = nullptr;
    Memory* Memory_ = nullptr;

    int m;
    int N;
    int P;
    int PID = 0;
    string cmd;
    string type_of_hashing;
    int position;
    int address;
    int val_write;

    while(cin >> cmd)
    {
        if(cmd == "OPEN" || cmd == "ORDERED")
        {
            type_of_hashing = cmd;
        }
        else if (cmd == "M")
        {
            cin >> N;
            cin >> P;
            m = N/P;

            hashtab = new Hash_table(m);
            Memory_ = new Memory(N, m, P);

            cout << "success" << endl;
        }
        else if (cmd == "INSERT")
        {
            cin >> PID;

            if (type_of_hashing == "OPEN") {
                if((hashtab->is_table_full_double_h() == true) || (hashtab->key_exists_double_h(PID) == true)) {
                    cout << "failure" << endl;
                }  

                else if((hashtab->is_table_full_double_h() == false) && (hashtab->key_exists_double_h(PID) == false)) {
                    int store_index = Memory_->alloc_page(PID);

                    if(store_index != -1) {
                        int memory_address = store_index * (Memory_->get_page_size());
                        hashtab->insert_double_h(PID, memory_address);
                        cout << "success" << endl;
                    }
                    else {
                        cout << "failure" << endl;
                    }
                }
            }

            else if (type_of_hashing == "ORDERED")
            {
                if((hashtab->get_size() == hashtab->get_capacity()) || (hashtab->key_exists_ordered_chain(PID) == true)) {
                    cout << "failure" << endl;
                }

                else if((hashtab->get_size() < hashtab->get_capacity()) && (hashtab->key_exists_ordered_chain(PID) == false)) {
                    int store_index = Memory_->alloc_page(PID);
                    
                    if(store_index != -1) {
                        int memory_address = store_index * (Memory_->get_page_size());
                        hashtab->insert_ordered_chain(PID, memory_address);
                        cout << "success" << endl;
                    }
                    else {
                        cout << "failure" << endl;
                    }
                }
            }
        }
        else if (cmd == "SEARCH")
        {
            cin >> PID;

            if (type_of_hashing == "OPEN") {
                int p = hashtab->search_PID_double_h(PID);
                if(p != -1)
                {
                    cout << "found " << PID << " in " << p << endl;
                }
                else
                {
                    cout << "not found" << endl;
                }
            }
            else if (type_of_hashing == "ORDERED") {
                int p = hashtab->search_PID_ordered_chain(PID);
                if(p != -1)
                {
                    cout << "found " << PID << " in " << p << endl;
                }
                else
                {
                    cout << "not found" << endl;
                }
            }
        }
        else if (cmd == "WRITE")
        {
            cin >> PID;
            cin >> address;
            cin >> val_write;

            bool addr_in_range = Memory_->addr_within_page(PID, address);

            if(type_of_hashing == "OPEN") {
                int exists = hashtab->search_PID_double_h(PID);

                if((exists != -1) && (addr_in_range == true))
                {
                    Memory_->write_val(PID, address, val_write);
                    cout << "success" << endl;
                }
                else if ((exists == -1) || (addr_in_range == false)) {
                    cout << "failure" << endl;
                }
            }
            else if (type_of_hashing == "ORDERED") {
                int exists_o = hashtab->search_PID_ordered_chain(PID);

                if((exists_o != -1) && (addr_in_range == true))
                {
                    Memory_->write_val(PID, address, val_write);
                    cout << "success" << endl;
                }
                else {
                    cout << "failure" << endl;
                }
            }
        }
        else if (cmd == "READ")
        {
            cin >> PID;
            cin >> address;

            bool addr_in_range = Memory_->addr_within_page(PID, address);

            if(type_of_hashing == "OPEN") {
                int exists = hashtab->search_PID_double_h(PID);

                if((exists != -1) && (addr_in_range == true))
                {
                    int x_ = Memory_->read_val(PID, address);
                    cout << address << " " << x_ << endl;
                }
                else {
                    cout << "failure" << endl;
                }
            }
            else if (type_of_hashing == "ORDERED") {
                int exists_o = hashtab->search_PID_ordered_chain(PID);

                if((exists_o != -1) && (addr_in_range == true))
                {
                    int x = Memory_->read_val(PID, address);
                    cout << address << " " << x << endl;
                }
                else {
                    cout << "failure" << endl;
                }
            }

        }
        else if (cmd == "DELETE")
        {
            cin >> PID;

            if (type_of_hashing == "OPEN") {
                if (hashtab->key_exists_double_h(PID) == false)
                {
                    cout << "failure" << endl;
                }
                else
                {
                    hashtab->delete_PID_double_h(PID);
                    Memory_->dealloc_page(PID);
                    cout << "success" << endl;
                }
            }
            else if (type_of_hashing == "ORDERED") {
                if(hashtab->key_exists_ordered_chain(PID) == false)
                {
                    cout << "failure" << endl;
                }
                else
                {
                    hashtab->delete_ordered_chain(PID);
                    Memory_->dealloc_page(PID);
                    cout << "success" << endl;
                }
            }

            
        }
        else if (cmd == "PRINT")
        {
            if(type_of_hashing == "ORDERED") {
                cin >> position;
                hashtab->print(position);
            }
            
        }
        else if(cmd == "END")
        {
            break;
        }
    }

    delete hashtab;
    delete Memory_;
    return 0;
}