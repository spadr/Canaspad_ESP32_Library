union Nucleus
{
private:
    float float_value;
    int int_value;
    long long_value;
    unsigned int unsigned_int_value;
    unsigned long unsigned_long_value;

public:
    Nucleus() {}
    ~Nucleus() {}
    void put_float(float f) { float_value = f; }
    void put_int(int i) { int_value = i; }
    void put_long(long l) { long_value = l; }
    void put_unsigned_int(unsigned int ui) { unsigned_int_value = ui; }
    void put_unsigned_long(unsigned long ul) { unsigned_long_value = ul; }
    float get_float() const { return float_value; }
    int get_int() const { return int_value; }
    long get_long() const { return long_value; }
    unsigned int get_unsigned_int() const { return unsigned_int_value; }
    unsigned long get_unsigned_long() const { return unsigned_long_value; }
};
