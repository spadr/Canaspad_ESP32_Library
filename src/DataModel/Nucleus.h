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
    void putFloat(float f) { float_value = f; }
    void putInt(int i) { int_value = i; }
    void putLong(long l) { long_value = l; }
    void putUnsignedInt(unsigned int ui) { unsigned_int_value = ui; }
    void putUnsignedLong(unsigned long ul) { unsigned_long_value = ul; }
    float getFloat() const { return float_value; }
    int getInt() const { return int_value; }
    long getLong() const { return long_value; }
    unsigned int getUnsignedInt() const { return unsigned_int_value; }
    unsigned long getUnsignedLong() const { return unsigned_long_value; }
};
