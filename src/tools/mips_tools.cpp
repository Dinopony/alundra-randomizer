#include "mips_tools.hpp"
#include "exception.hpp"

MipsCode::~MipsCode() noexcept(false)
{
    if(!_pending_branches.empty())
        throw RandomizerException("Unresolved labels are remaining in code");
}

MipsCode& MipsCode::lui(const MipsRegister& reg, uint16_t value)
{
    // 0011 1100 000t tttt iiii iiii iiii iiii
    uint32_t opcode = 0x3C000000;
    opcode |= reg.code() << 16;
    opcode |= value;

    this->add_long_le(opcode);
    return *this;
}

MipsCode& MipsCode::slt(const MipsRegister& reg_to, const MipsRegister& reg_from_1, const MipsRegister& reg_from_2)
{
    // 0000 00ss ssst tttt dddd d000 0010 1010
    uint32_t opcode = 0x0000002A;
    opcode |= reg_from_1.code() << 21;
    opcode |= reg_from_2.code() << 16;
    opcode |= reg_to.code() << 11;

    this->add_long_le(opcode);
    return *this;
}

MipsCode& MipsCode::slti(const MipsRegister& reg_to, const MipsRegister& reg_from, uint16_t value)
{
    // 0010 10ss ssst tttt iiii iiii iiii iiii
    uint32_t opcode = 0x28000000;
    opcode |= reg_from.code() << 21;
    opcode |= reg_to.code() << 16;
    opcode |= value;

    this->add_long_le(opcode);
    return *this;
}

MipsCode& MipsCode::sw(const MipsRegister& reg_from, const MipsRegister& reg_to, uint16_t offset_to)
{
    // 1010 11ss ssst tttt iiii iiii iiii iiii
    uint32_t opcode = 0xAC000000;
    opcode |= reg_to.code() << 21;
    opcode |= reg_from.code() << 16;
    opcode |= offset_to;

    this->add_long_le(opcode);
    return *this;
}

MipsCode& MipsCode::sh(const MipsRegister& reg_from, const MipsRegister& reg_to, uint16_t offset_to)
{
    // 1010 01ss ssst tttt iiii iiii iiii iiii
    uint32_t opcode = 0xA4000000;
    opcode |= reg_to.code() << 21;
    opcode |= reg_from.code() << 16;
    opcode |= offset_to;

    this->add_long_le(opcode);
    return *this;
}

MipsCode& MipsCode::sb(const MipsRegister& reg_from, const MipsRegister& reg_to, uint16_t offset_to)
{
    // 1010 00ss ssst tttt iiii iiii iiii iiii
    uint32_t opcode = 0xA0000000;
    opcode |= reg_to.code() << 21;
    opcode |= reg_from.code() << 16;
    opcode |= offset_to;

    this->add_long_le(opcode);
    return *this;
}

MipsCode& MipsCode::lw(const MipsRegister& reg_to, const MipsRegister& reg_from, uint16_t offset_from, bool add_load_delay_nop)
{
    // 1000 11ss ssst tttt iiii iiii iiii iiii
    uint32_t opcode = 0x8C000000;
    opcode |= reg_from.code() << 21;
    opcode |= reg_to.code() << 16;
    opcode |= offset_from;

    this->add_long_le(opcode);
    if(add_load_delay_nop)
        this->nop();
    return *this;
}

MipsCode& MipsCode::lh(const MipsRegister& reg_to, const MipsRegister& reg_from, uint16_t offset_from, bool add_load_delay_nop)
{
    // 1000 01ss ssst tttt iiii iiii iiii iiii
    uint32_t opcode = 0x84000000;
    opcode |= reg_from.code() << 21;
    opcode |= reg_to.code() << 16;
    opcode |= offset_from;

    this->add_long_le(opcode);
    if(add_load_delay_nop)
        this->nop();
    return *this;
}

MipsCode& MipsCode::lb(const MipsRegister& reg_to, const MipsRegister& reg_from, uint16_t offset_from, bool add_load_delay_nop)
{
    // 1000 00ss ssst tttt iiii iiii iiii iiii
    uint32_t opcode = 0x80000000;
    opcode |= reg_from.code() << 21;
    opcode |= reg_to.code() << 16;
    opcode |= offset_from;

    this->add_long_le(opcode);
    if(add_load_delay_nop)
        this->nop();
    return *this;
}

MipsCode& MipsCode::j(uint32_t addr, bool add_load_delay_nop)
{
    // 0000 10ii iiii iiii iiii iiii iiii iiii
    uint32_t opcode = 0x08000000;
    opcode |= (addr / 4) & 0x00FFFFFF;

    this->add_long_le(opcode);

    // Add a load slot delay nop
    if(add_load_delay_nop)
        this->nop();

    return *this;
}

MipsCode& MipsCode::jal(uint32_t addr, bool add_load_delay_nop)
{
    // 0000 11ii iiii iiii iiii iiii iiii iiii
    uint32_t opcode = 0x0C000000;
    opcode |= (addr / 4) & 0x00FFFFFF;

    this->add_long_le(opcode);

    // Add a load slot delay nop
    if(add_load_delay_nop)
        this->nop();

    return *this;
}

MipsCode& MipsCode::jr(const MipsRegister& reg, bool add_load_delay_nop)
{
    // 0000 00ss sss0 0000 0000 0000 0000 1000
    uint32_t opcode = 0x00000008;
    opcode |= (reg.code() << 21);
    this->add_long_le(opcode);

    // Add a load slot delay nop
    if(add_load_delay_nop)
        this->nop();

    return *this;
}

MipsCode& MipsCode::nop()
{
    this->add_long(0x00000000);
    return *this;
}

MipsCode& MipsCode::add(const MipsRegister& reg_to, const MipsRegister& reg_from_1, const MipsRegister& reg_from_2)
{
    // 0000 00ss ssst tttt dddd	d000 0010 0000
    uint32_t opcode = 0x00000020;
    opcode |= reg_from_1.code() << 21;
    opcode |= reg_from_2.code() << 16;
    opcode |= reg_to.code() << 11;

    this->add_long_le(opcode);
    return *this;
}

MipsCode& MipsCode::addu(const MipsRegister& reg_to, const MipsRegister& reg_from_1, const MipsRegister& reg_from_2)
{
    // 0000 00ss ssst tttt dddd d000 0010 0001
    uint32_t opcode = 0x00000021;
    opcode |= reg_from_1.code() << 21;
    opcode |= reg_from_2.code() << 16;
    opcode |= reg_to.code() << 11;

    this->add_long_le(opcode);
    return *this;
}

MipsCode& MipsCode::addiu(const MipsRegister& reg_to, const MipsRegister& reg_from, uint16_t value)
{
    // 0010 01ss ssst tttt iiii iiii iiii iiii
    uint32_t opcode = 0x24000000;
    opcode |= reg_from.code() << 21;
    opcode |= reg_to.code() << 16;
    opcode |= value;

    this->add_long_le(opcode);
    return *this;
}

MipsCode& MipsCode::mult(const MipsRegister& reg_1, const MipsRegister& reg_2)
{
    // 0000 00ss ssst tttt 0000 0000 0001 1000
    uint32_t opcode = 0x00000018;
    opcode |= reg_1.code() << 21;
    opcode |= reg_2.code() << 16;

    this->add_long_le(opcode);
    return *this;
}

MipsCode& MipsCode::multu(const MipsRegister& reg_1, const MipsRegister& reg_2)
{
    // 0000 00ss ssst tttt 0000 0000 0001 1001
    uint32_t opcode = 0x00000019;
    opcode |= reg_1.code() << 21;
    opcode |= reg_2.code() << 16;

    this->add_long_le(opcode);
    return *this;
}

MipsCode& MipsCode::mflo(const MipsRegister& reg)
{
    // 0000 0000 0000 0000 dddd d000 0001 0010
    uint32_t opcode = 0x00000012;
    opcode |= reg.code() << 11;

    this->add_long_le(opcode);
    return *this;
}

MipsCode& MipsCode::logical_and(const MipsRegister& reg_to, const MipsRegister& reg_from_1, const MipsRegister& reg_from_2)
{
    // 0000 00ss ssst tttt dddd d000 0010 0100
    uint32_t opcode = 0x00000024;
    opcode |= reg_from_1.code() << 21;
    opcode |= reg_from_2.code() << 16;
    opcode |= reg_to.code() << 11;

    this->add_long_le(opcode);
    return *this;
}

MipsCode& MipsCode::beq(const MipsRegister& reg_1, const MipsRegister& reg_2, const std::string& label)
{
    _pending_branches[this->size()] = label;

    // 0001 01ss ssst tttt ffff ffff ffff ffff
    uint32_t opcode = 0x10000000;
    opcode |= reg_1.code() << 21;
    opcode |= reg_2.code() << 16;

    this->add_long_le(opcode);
    this->resolve_branches();
    this->nop();
    return *this;
}

MipsCode& MipsCode::bne(const MipsRegister& reg_1, const MipsRegister& reg_2, const std::string& label)
{
    _pending_branches[this->size()] = label;

    // 0001 01ss ssst tttt ffff ffff ffff ffff
    uint32_t opcode = 0x14000000;
    opcode |= reg_1.code() << 21;
    opcode |= reg_2.code() << 16;

    this->add_long_le(opcode);
    this->resolve_branches();
    this->nop();
    return *this;
}

MipsCode& MipsCode::bgez(const MipsRegister& reg, const std::string& label)
{
    _pending_branches[this->size()] = label;

    // 0000 01ss sss0 0001 ffff ffff ffff ffff
    uint32_t opcode = 0x04010000;
    opcode |= reg.code() << 21;

    this->add_long_le(opcode);
    this->resolve_branches();
    this->nop();
    return *this;
}

// =======================================================================
//      PSEUDO INSTRUCTIONS
// =======================================================================

MipsCode& MipsCode::set_(const MipsRegister& reg_to, uint32_t value)
{
    uint16_t upper_half = value >> 16;
    uint16_t lower_half = value & 0xFFFF;
    if(lower_half & 0x8000)
        upper_half += 1;

    if(upper_half)
    {
        this->lui(reg_to, upper_half);
        this->addiu(reg_to, reg_to, lower_half);
    }
    else
    {
        this->addiu(reg_to, reg_ZERO, lower_half);
    }

    return *this;
}

MipsCode& MipsCode::set_(const MipsRegister& reg_to, const MipsRegister& reg_from)
{
    this->add(reg_to, reg_from, reg_ZERO);
    return *this;
}

MipsCode& MipsCode::multiply_(const MipsRegister& reg_dest, const MipsRegister& reg_src, uint16_t value)
{
    this->set_(reg_T9, value);
    this->mult(reg_src, reg_T9);
    this->mflo(reg_dest);
    return *this;
}

MipsCode& MipsCode::multiply_(const MipsRegister& reg_dest, const MipsRegister& reg_1, const MipsRegister& reg_2)
{
    this->mult(reg_1, reg_2);
    this->mflo(reg_dest);
    return *this;
}

MipsCode& MipsCode::clr_(const MipsRegister& reg)
{
    this->logical_and(reg, reg, reg_ZERO);
    return *this;
}

MipsCode& MipsCode::label_(const std::string& label)
{
    _labels[label] = static_cast<uint32_t>(this->size());
    this->resolve_branches();
    return *this;
}

MipsCode& MipsCode::bra_(const std::string& label)
{
    this->bgez(reg_ZERO, label);
    return *this;
}

MipsCode& MipsCode::beq_(const MipsRegister& reg_1, uint32_t value, const std::string& label)
{
    this->set_(reg_T9, value);
    return this->beq(reg_1, reg_T9, label);
}

MipsCode& MipsCode::bne_(const MipsRegister& reg_1, uint32_t value, const std::string& label)
{
    this->set_(reg_T9, value);
    return this->bne(reg_1, reg_T9, label);
}

MipsCode& MipsCode::ble_(const MipsRegister& reg_1, const MipsRegister& reg_2, const std::string& label)
{
    // if(reg1 <= reg2) <-------> if(!(reg2 < reg1))
    this->slt(reg_T9, reg_2, reg_1);    // reg_2 < reg_1
    this->beq(reg_T9, reg_ZERO, label); // branch if false
    return *this;
}

MipsCode& MipsCode::ble_(const MipsRegister& reg_1, uint32_t value, const std::string& label)
{
    this->set_(reg_T9, value);
    return this->ble_(reg_1, reg_T9, label);
}

MipsCode& MipsCode::blt_(const MipsRegister& reg_1, const MipsRegister& reg_2, const std::string& label)
{
    this->slt(reg_T9, reg_1, reg_2);    // reg_1 < reg_2
    this->bne(reg_T9, reg_ZERO, label); // branch if true
    return *this;
}

MipsCode& MipsCode::blt_(const MipsRegister& reg_1, uint32_t value, const std::string& label)
{
    this->slti(reg_T9, reg_1, value);   // reg_1 < value
    this->bne(reg_T9, reg_ZERO, label); // branch if true
    return *this;
}

MipsCode& MipsCode::bge_(const MipsRegister& reg_1, const MipsRegister& reg_2, const std::string& label)
{
    // if(reg1 >= reg2) <-------> if(!(reg1 < reg2))
    this->slt(reg_T9, reg_1, reg_2);    // reg_1 < reg_2
    this->beq(reg_T9, reg_ZERO, label); // branch if false
    return *this;
}

MipsCode& MipsCode::bge_(const MipsRegister& reg_1, uint32_t value, const std::string& label)
{
    this->set_(reg_T9, value);
    return this->bge_(reg_1, reg_T9, label);
}

MipsCode& MipsCode::bgt_(const MipsRegister& reg_1, const MipsRegister& reg_2, const std::string& label)
{
    // if(reg1 > reg2) <-------> if(reg2 < reg1)
    return this->blt_(reg_2, reg_1, label);
}

MipsCode& MipsCode::bgt_(const MipsRegister& reg_1, uint32_t value, const std::string& label)
{
    this->set_(reg_T9, value);
    return this->bgt_(reg_1, reg_T9, label);
}


void MipsCode::resolve_branches()
{
    std::vector<uint32_t> branches_to_clean;

    for (auto& [branch_address, label] : _pending_branches)
    {
        if (!_labels.count(label))
            continue;

        // Find the offset between the label address and the branch instruction address
        uint32_t label_address = _labels[label];
        int32_t offset_in_longs = (int32_t)(label_address - (branch_address + 4)) / 4;
        int16_t truncated_offset_in_longs = static_cast<int16_t>(offset_in_longs);

        this->set_word_le(branch_address, static_cast<uint16_t>(truncated_offset_in_longs));
        branches_to_clean.emplace_back(branch_address);
    }

    for (uint32_t addr : branches_to_clean)
        _pending_branches.erase(addr);
}
