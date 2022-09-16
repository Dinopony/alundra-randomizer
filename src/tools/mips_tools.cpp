#include "mips_tools.hpp"
#include <stdexcept>

MipsCode& MipsCode::li(const MipsRegister& reg, uint16_t value)
{
    this->add_word_le(value);
    this->add_byte(reg.code());
    this->add_byte(0x24);
    return *this;
}

MipsCode& MipsCode::lui(const MipsRegister& reg, uint16_t value)
{
    this->add_word_le(value);
    this->add_byte(reg.code());
    this->add_byte(0x3C);
    return *this;
}

MipsCode& MipsCode::sw(const MipsRegister& reg_from, const MipsRegister& reg_to, uint16_t offset_to)
{
    this->add_word_le(offset_to);

    // Opcode : 1010 11DD DDDS SSSS
    uint16_t opcode = 0xAC00;
    opcode |= reg_to.code() << 5;
    opcode |= reg_from.code();
    this->add_word_le(opcode);

    return *this;
}

MipsCode& MipsCode::sh(const MipsRegister& reg_from, const MipsRegister& reg_to, uint16_t offset_to)
{
    this->add_word_le(offset_to);

    // Opcode : 1010 10DD DDDS SSSS
    uint16_t opcode = 0xA400;
    opcode |= reg_to.code() << 5;
    opcode |= reg_from.code();
    this->add_word_le(opcode);

    return *this;
}

MipsCode& MipsCode::sb(const MipsRegister& reg_from, const MipsRegister& reg_to, uint16_t offset_to)
{
    this->add_word_le(offset_to);

    // Opcode : 1010 00DD DDDS SSSS
    uint16_t opcode = 0xA000;
    opcode |= reg_to.code() << 5;
    opcode |= reg_from.code();
    this->add_word_le(opcode);

    return *this;
}

MipsCode& MipsCode::j(uint32_t addr, bool add_load_delay_nop)
{
    uint32_t long_count = (addr / 4) & 0x00FFFFFF;
    uint32_t instruction = 0x08000000 | long_count;
    this->add_long_le(instruction);

    // Add a load slot delay nop
    if(add_load_delay_nop)
        this->nop();

    return *this;
}

MipsCode& MipsCode::jr(const MipsRegister& reg, bool add_load_delay_nop)
{
    uint32_t instruction = 0x00000008;
    instruction |= (reg.code() << 21);
    this->add_long_le(instruction);

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

// =======================================================================

/*
void MipsCode::label(const std::string& label)
{
    _labels[label] = static_cast<uint32_t>(_bytes.size());
    this->resolve_branches();
}

const std::vector<uint8_t>& MipsCode::get_bytes() const
{
    if (!_pending_branches.empty())
    {
        std::string msg = std::string("Pending branch is unresolved on injected code : ")
                         + _pending_branches.begin()->second;
        throw std::exception(msg.c_str());
    }

    return _bytes;
}

void MipsCode::resolve_branches()
{
    std::vector<uint32_t> branches_to_clean;

    for (auto& [branch_address, label] : _pending_branches)
    {
        if (_labels.count(label))
        {
            uint32_t label_address = _labels[label];
            int32_t offset = (int32_t)(label_address - branch_address);
            if(offset > 0x7FFF || offset < -0x7FFF)
            {
                std::string msg = "Offset for branch at byte " + std::to_string(branch_address) +
                                  " is too big (cannot be expressed as word)";
                throw std::exception(msg.c_str());
            }

            int16_t truncated_offset = (int16_t)(label_address - branch_address);
            uint16_t truncated_offset_as_word = static_cast<uint16_t>(truncated_offset);
            if (truncated_offset > 0x7F || truncated_offset < -0x7F)
            {
                // Branch offset is more than one byte long, we need to add an optional displacement
                _bytes.insert(_bytes.begin() + branch_address, static_cast<uint8_t>(truncated_offset_as_word >> 8));
                _bytes.insert(_bytes.begin() + branch_address + 1, static_cast<uint8_t>(truncated_offset_as_word & 0x00FF));
            }
            else
            {
                // Branch offset is less than one byte long, we only need to increment opcode by the offset value
                _bytes[static_cast<size_t>(branch_address) - 1] += truncated_offset_as_word;
            }

            branches_to_clean.emplace_back(branch_address);
        }
    }

    for (uint32_t addr : branches_to_clean)
        _pending_branches.erase(addr);
}
*/