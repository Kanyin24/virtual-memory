#ifndef HEAD
#define HEAD
#include <iostream>
#include <vector>

class Process
{
    public:
        Process();
        ~Process();

        //getters
        int get_PID();
        int get_physical_addr();
        bool get_delete_flag();

        //setters
        void set_PID(int PID);
        void set_physical_addr(int physical_add);
        void set_delete_flag(bool delete_flag);

    private:
        int PID_ = 0;
        int physical_addr_;
        bool delete_flag_;
};

class Memory
{
    public:
        Memory(int mem_size, int allo_size, int page_size);
        ~Memory();

        //getter
        int get_alloc_size();
        int get_page_size();

        int search(int PID);
        int alloc_page(int PID);
        void write_val(int PID, int addr, int x);
        bool addr_within_page(int PID, int addr);
        int read_val(int PID, int addr);
        void dealloc_page(int PID);


    private:
        int mem_size_;
        int page_size_;
        int* Memory_storage;
        int alloc_size_;
        int* alloc;
};

class Hash_table
{
    public:
        Hash_table(int capacity);
        ~Hash_table();

        //getters
        int get_size();
        int get_capacity();

        //double hashing technique functions
        void insert_double_h(int PID, int physical_address);
        int search_PID_double_h(int PID);
        bool is_table_full_double_h();
        bool key_exists_double_h(int PID);
        void delete_PID_double_h(int PID);

        //chaining technique functions
        void insert_ordered_chain(int PID, int physical_address);
        int search_PID_ordered_chain(int PID);
        bool key_exists_ordered_chain(int PID);
        void delete_ordered_chain(int PID);
        void print(int position);

    private:
        int size_;
        int capacity_;
        Process* array;
        std::vector <Process>* chaining_v_array;
};


#endif