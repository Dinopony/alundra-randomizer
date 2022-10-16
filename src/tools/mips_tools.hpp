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
    virtual ~MipsCode() noexcept(false);

    MipsCode& nop();

    // ----- ARITHMETIC LOGIC UNIT -----
    MipsCode& add(const MipsRegister& reg_to, const MipsRegister& reg_from_1, const MipsRegister& reg_from_2);
    MipsCode& addu(const MipsRegister& reg_to, const MipsRegister& reg_from_1, const MipsRegister& reg_from_2);
//  MipsCode& addi(const MipsRegister& reg_to, const MipsRegister& reg_from, uint16_t value);
    MipsCode& addiu(const MipsRegister& reg_to, const MipsRegister& reg_from, uint16_t value);
    MipsCode& addiu(const MipsRegister& reg, uint16_t value) { return this->addiu(reg, reg, value); }

    MipsCode& logical_and(const MipsRegister& reg_to, const MipsRegister& reg_from_1, const MipsRegister& reg_from_2);
//  MipsCode& andi(const MipsRegister& reg_to, const MipsRegister& reg_from, uint16_t value);

    MipsCode& lui(const MipsRegister& reg, uint16_t value);

//  MipsCode& nor(const MipsRegister& reg_to, const MipsRegister& reg_from_1, const MipsRegister& reg_from_2);
//  MipsCode& or(const MipsRegister& reg_to, const MipsRegister& reg_from_1, const MipsRegister& reg_from_2);
//  MipsCode& ori(const MipsRegister& reg_to, const MipsRegister& reg_from, uint16_t value);

    MipsCode& slt(const MipsRegister& reg_to, const MipsRegister& reg_from_1, const MipsRegister& reg_from_2);
//  MipsCode& sltu(const MipsRegister& reg_to, const MipsRegister& reg_from_1, const MipsRegister& reg_from_2);
    MipsCode& slti(const MipsRegister& reg_to, const MipsRegister& reg_from, uint16_t value);
//  MipsCode& sltiu(const MipsRegister& reg_to, const MipsRegister& reg_from, uint16_t value);

//  MipsCode& sub(const MipsRegister& reg_to, const MipsRegister& reg_from_1, const MipsRegister& reg_from_2);
//  MipsCode& subu(const MipsRegister& reg_to, const MipsRegister& reg_from_1, const MipsRegister& reg_from_2);

//  MipsCode& xor(const MipsRegister& reg_to, const MipsRegister& reg_from_1, const MipsRegister& reg_from_2);
//  MipsCode& xori(const MipsRegister& reg_to, const MipsRegister& reg_from, uint16_t value);

    // ----- MULTIPLY -----
    MipsCode& mult(const MipsRegister& reg_1, const MipsRegister& reg_2);
    MipsCode& multu(const MipsRegister& reg_1, const MipsRegister& reg_2);
    MipsCode& mflo(const MipsRegister& reg);

    // ----- BRANCH -----
    MipsCode& beq(const MipsRegister& reg_1, const MipsRegister& reg_2, const std::string& label);
    MipsCode& bne(const MipsRegister& reg_1, const MipsRegister& reg_2, const std::string& label);
    MipsCode& bgez(const MipsRegister& reg, const std::string& label);

    MipsCode& j(uint32_t addr, bool add_load_delay_nop = true);
    MipsCode& jal(uint32_t addr, bool add_load_delay_nop = true);
    MipsCode& jr(const MipsRegister& reg, bool add_load_delay_nop = true);

    // ----- MEMORY ACCESS -----
    MipsCode& sw(const MipsRegister& reg_from, const MipsRegister& reg_to, uint16_t offset_to = 0);
    MipsCode& sh(const MipsRegister& reg_from, const MipsRegister& reg_to, uint16_t offset_to = 0);
    MipsCode& sb(const MipsRegister& reg_from, const MipsRegister& reg_to, uint16_t offset_to = 0);
    MipsCode& lw(const MipsRegister& reg_to, const MipsRegister& reg_from, uint16_t offset_from = 0, bool add_load_delay_nop = true);
    MipsCode& lh(const MipsRegister& reg_to, const MipsRegister& reg_from, uint16_t offset_from = 0, bool add_load_delay_nop = true);
    MipsCode& lb(const MipsRegister& reg_to, const MipsRegister& reg_from, uint16_t offset_from = 0, bool add_load_delay_nop = true);

    // ----- PSEUDO INSTRUCTIONS -----
    MipsCode& set_(const MipsRegister& reg_to, uint32_t value);
    MipsCode& set_(const MipsRegister& reg_to, const MipsRegister& reg_from);

    MipsCode& multiply_(const MipsRegister& reg_dest, const MipsRegister& reg_src, uint16_t value);
    MipsCode& multiply_(const MipsRegister& reg_dest, const MipsRegister& reg_1, const MipsRegister& reg_2);

    MipsCode& clr_(const MipsRegister& reg);
    void clear() { throw std::exception(); }

    MipsCode& label_(const std::string& label);
    MipsCode& bra_(const std::string& label);
    MipsCode& beq_(const MipsRegister& reg_1, uint32_t value, const std::string& label);
    MipsCode& bne_(const MipsRegister& reg_1, uint32_t value, const std::string& label);
    MipsCode& ble_(const MipsRegister& reg_1, const MipsRegister& reg_2, const std::string& label);
    MipsCode& ble_(const MipsRegister& reg_1, uint32_t value, const std::string& label);
    MipsCode& blt_(const MipsRegister& reg_1, const MipsRegister& reg_2, const std::string& label);
    MipsCode& blt_(const MipsRegister& reg_1, uint32_t value, const std::string& label);
    MipsCode& bge_(const MipsRegister& reg_1, const MipsRegister& reg_2, const std::string& label);
    MipsCode& bge_(const MipsRegister& reg_1, uint32_t value, const std::string& label);
    MipsCode& bgt_(const MipsRegister& reg_1, const MipsRegister& reg_2, const std::string& label);
    MipsCode& bgt_(const MipsRegister& reg_1, uint32_t value, const std::string& label);

private:
    void resolve_branches();

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
