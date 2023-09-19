#include "header_hash.h"
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;


//Double hashing

//Process class functions
Process::Process()
{
    PID_ = 0;
    physical_addr_ = -1;
    delete_flag_ = false;
}

Process::~Process() 
{

}

int Process::get_PID() 
{
    return PID_;
}

int Process::get_physical_addr() {
    return physical_addr_;
}

bool Process::get_delete_flag() {
    return delete_flag_;
}

void Process::set_physical_addr(int physical_add) {
    physical_addr_ = physical_add;
}

void Process::set_PID(int PID) {
    PID_ = PID;
}

void Process::set_delete_flag(bool delete_flag) {
    delete_flag_ = delete_flag;
}

//Page_Memory class functions
Memory::Memory(int mem_size, int allo_size, int page_size) 
{
    mem_size_ = mem_size;
    Memory_storage = new int[mem_size_];
    alloc_size_ = allo_size;
    alloc = new int[allo_size];
    page_size_ = page_size;

    for (int i = 0; i < alloc_size_; i++) {
        alloc[i] = 0;

    }
}

Memory::~Memory() {
    delete[] Memory_storage;
    delete[] alloc;
}

int Memory::search(int PID) {
    for (int i = 0; i < get_alloc_size(); i++) {

        if (alloc[i] == PID) {
            return i;
        }
    }
    return -1;
}

int Memory::alloc_page(int PID) {

    int virt_addr;

    for (int i = 0; i < get_alloc_size(); i++) {

        if (alloc[i] == 0) {
            alloc[i] = PID;
            virt_addr = i * get_page_size();
            return virt_addr;
        }
    }
    return -1;
}

void Memory::write_val(int PID, int addr, int x) {

    int i = search(PID);
    Memory_storage[(i * get_page_size()) + addr] = x;
}

bool Memory::addr_within_page(int PID, int addr) {
    int i = search(PID);

    if(((i * get_page_size()) + addr) < ((i * get_page_size()) + get_page_size())) {
        return true;
    }
    else {
        return false;
    }
}

int Memory::read_val(int PID, int addr) {
    int i = search(PID);
    return Memory_storage[(i * get_page_size()) + addr];
}

void Memory::dealloc_page(int PID) {
    int i = search(PID);

    alloc[i] = 0;

}

int Memory::get_alloc_size() {
    return alloc_size_;
}

int Memory::get_page_size() {
    return page_size_;
}

//Hash_table class functions
Hash_table::Hash_table(int capacity) 
{
    size_ = 0;
    capacity_ = capacity;
    array = new Process[capacity_];

    chaining_v_array = new std::vector <Process> [capacity_];
}

Hash_table::~Hash_table() {
    delete[] array;
    delete[] chaining_v_array;
}

int Hash_table::get_size() {
    return size_;
}

int Hash_table::get_capacity() {
    return capacity_;
}

void Hash_table::insert_double_h(int PID, int physical_address)
{

    int h1 = (int) PID % capacity_;
    int index_location;
    int h2 = (int) (floor(PID/capacity_)) % capacity_;  
    
    if(h2 % 2 == 0) {
        h2 += 1;
    }
    else {
        h2 = h2;
    }

    for(int i = 0; i < capacity_; i++)
    {
        index_location = (h1 + (i*h2)) % capacity_;

        if(array[index_location].get_PID() == 0)
        {
            array[index_location].set_PID(PID);
            array[index_location].set_physical_addr(physical_address);

            size_++;
            break;
        }
    }
    
}

int Hash_table::search_PID_double_h(int PID)
{
    int h1 = (int) PID % capacity_;
    int index_location;
    int h2 = (int) (floor(PID/capacity_)) % capacity_;

    if(h2 % 2 == 0) {
        h2 += 1;
    }
    else {
        h2 = h2;
    }

    for(int i = 0; i < capacity_; i++)
    {
        index_location = (h1 + (i*h2)) % capacity_;
        if(array[index_location].get_PID() == 0 && array[index_location].get_delete_flag() == false) {
            return -1;
        }
        else {
            if(array[index_location].get_PID() == PID)
            {
                return index_location;
            }
        }
    }
    return -1;
    
}

bool Hash_table::is_table_full_double_h() {
    int check_size = 0;
    for(int i = 0; i < capacity_; i++)
    {
        if(array[i].get_PID() != 0) {
            check_size++;
        }
    }
    if(check_size == capacity_)
    {
        return true;
    }
    else {
        return false;
    }
}

bool Hash_table::key_exists_double_h(int PID) {
    for(int i = 0; i < capacity_; i++)
    {
        if(array[i].get_PID() == PID) {
            return true;
        }
    }
    return false;
}

void Hash_table::delete_PID_double_h(int PID) {
    int index_val = search_PID_double_h(PID);
    array[index_val].set_PID(0);
    array[index_val].set_delete_flag(true);
    array[index_val].set_physical_addr(-1);
}

void Hash_table::insert_ordered_chain(int PID, int physical_address) {
    Process p;
    p.set_PID(PID);
    p.set_physical_addr(physical_address);

    int index_val = PID % capacity_;

    

    if(chaining_v_array[index_val].size() == 0)
    {
        chaining_v_array[index_val].push_back(p);
        size_++;
            
    }
    else {
        bool itemFound = false;
        for (auto it = chaining_v_array[index_val].begin(); it != chaining_v_array[index_val].end(); it++)
        {
            if(it->get_PID() < p.get_PID())
            {
                itemFound = true;
                chaining_v_array[index_val].insert(it, p);
                size_++;
                break;
            }
        }
        if(itemFound == false) {
            chaining_v_array[index_val].push_back(p);
            size_++;
        }

    }
}

int Hash_table::search_PID_ordered_chain(int PID) {

    for (int i = 0; i < capacity_; i++) {
        for (auto it = chaining_v_array[i].begin(); it != chaining_v_array[i].end(); it++)
        {
            if(it->get_PID() == PID)
            {
                return i;
            }
        }
    }
    return -1;
}

bool Hash_table::key_exists_ordered_chain(int PID) {
    
    for (int i = 0; i < capacity_; i++) {
        for (auto it = chaining_v_array[i].begin(); it != chaining_v_array[i].end(); it++)
        {
            if(it->get_PID() == PID)
            {
                return true;
            }
        }
    }
    return false;
}

void Hash_table::delete_ordered_chain(int PID) {
    int i = search_PID_ordered_chain(PID);

    for(int j = 0; j < chaining_v_array[i].size(); j++) {
        if(chaining_v_array[i][j].get_PID() == PID)
        {
            chaining_v_array[i].erase(chaining_v_array[i].begin() + j);
            size_--;
            break;
        }
    }
}

void Hash_table::print(int position) {
    if (chaining_v_array[position].size() == 0) {
        std::cout << "chain is empty" << endl;
    }
    else {
        for (auto it = chaining_v_array[position].begin(); it != chaining_v_array[position].end(); it++)
        {
            std::cout << (*it).get_PID() << ' ';
                
        }
        std::cout << endl;
    }

}