#pragma once

#include <vector>
#include <map>
#include <string>
#include "byte_array.hpp"

////////////////////////////////////////////////////////////////////////

class MipsRegister {
public:
    constexpr explicit MipsRegister(uint8_t code) : _code(code)
    {}

    [[nodiscard]] uint8_t code() const { return _code; }

private:
    uint8_t _code;
};

////////////////////////////////////////////////////////////////////////

class MipsCode : public ByteArray {
private:
    std::map<uint32_t, std::string> _pending_branches;  // key = address of branch instruction, value = label name
    std::map<std::string, uint32_t> _labels;

public:
    MipsCode() = default;

    MipsCode& li(const MipsRegister& reg, uint16_t value);
    MipsCode& lui(const MipsRegister& reg, uint16_t value);
    MipsCode& sw(const MipsRegister& reg_from, const MipsRegister& reg_to, uint16_t offset_to);
    MipsCode& sh(const MipsRegister& reg_from, const MipsRegister& reg_to, uint16_t offset_to);
    MipsCode& sb(const MipsRegister& reg_from, const MipsRegister& reg_to, uint16_t offset_to);
    MipsCode& j(uint32_t addr, bool add_load_delay_nop = true);
    MipsCode& jr(const MipsRegister& reg, bool add_load_delay_nop = true);
    MipsCode& nop();

    /*
    void label(const std::string& label);
    [[nodiscard]] const std::vector<uint8_t>& get_bytes() const;

private:
    void resolve_branches();
    */
};

#define reg_ZERO MipsRegister(0)
#define reg_AT MipsRegister(1)

#define reg_V0 MipsRegister(2)
#define reg_V1 MipsRegister(3)

#define reg_A0 MipsRegister(4)
#define reg_A1 MipsRegister(5)
#define reg_A2 MipsRegister(6)
#define reg_A3 MipsRegister(7)

#define reg_T0 MipsRegister(8)
#define reg_T1 MipsRegister(9)
#define reg_T2 MipsRegister(10)
#define reg_T3 MipsRegister(11)
#define reg_T4 MipsRegister(12)
#define reg_T5 MipsRegister(13)
#define reg_T6 MipsRegister(14)
#define reg_T7 MipsRegister(15)

#define reg_S0 MipsRegister(16)
#define reg_S1 MipsRegister(17)
#define reg_S2 MipsRegister(18)
#define reg_S3 MipsRegister(19)
#define reg_S4 MipsRegister(20)
#define reg_S5 MipsRegister(21)
#define reg_S6 MipsRegister(22)
#define reg_S7 MipsRegister(23)

#define reg_T8 MipsRegister(24)
#define reg_T9 MipsRegister(25)

#define reg_K0 MipsRegister(26)
#define reg_K1 MipsRegister(27)

#define reg_GP MipsRegister(28)
#define reg_SP MipsRegister(29)
#define reg_FP MipsRegister(30)
#define reg_RA MipsRegister(31)
