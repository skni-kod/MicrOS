#ifndef CPUID_0x01H_H
#define CPUID_0x01H_H

#include <stdint.h>

//! CPU's stepping, model, and family information
typedef struct cpuid_0x01h_eax_fields
{
    //! Stepping ID
    uint8_t stepping_id : 4;
    //! Model
    uint8_t model : 4;
    //! Family ID
    uint8_t family_id : 4;
    //! Procesor type
    /*! <table>
    <tr><th>Type</th> <th>Encoding in Binary</th></tr>
    <tr><td>Original OEM Processor</td> <td>00</td></tr>
    <tr><td>Intel Overdrive Processor</td> <td>01</td></tr>
    <tr><td>Dual processor (not applicable to Intel486 processors)</td> <td>10</td></tr>
    <tr><td>Reserved value</td> <td>11</td></tr>
    </table>*/
    uint8_t procesor_type : 2;
    // Reserved
    uint8_t : 2;
    //! Extended model ID
    uint8_t extended_model_id : 4;
    //! Extended family ID
    uint8_t extended_family_id : 8;
    // Reserved
    uint8_t : 4;
} __attribute__((packed)) cpuid_0x01h_eax_fields;

//! CPU additional information.
typedef struct cpuid_0x01h_ebx_fields
{
    //! Brand index.
    uint8_t brand_index;
    //! CLFLUSH line size
    /*! Valid if CLFLUSH feature flag is set.<br/>
    CPUID.01.EDX.CLFSH [bit 19]= 1*/
    uint8_t clflush_line_size;
    //! Maximum number of addressable IDs for logical processors in this physical package.
    /*!
    The nearest power-of-2 integer that is not smaller than this value is the number of unique initial APIC IDs reserved
    for addressing different logical processors in a physical package. <br/>
    Former use: Number of logical processors per physical processor, two for the Pentium 4 processor with Hyper-Threading Technology.<br/>
    Valid if Hyper-threading feature flag is set.<br/>
    CPUID.01.EDX.HTT [bit 28]= 1 */
    uint8_t max_number_of_addressable_ids;
    //! Local APIC ID
    /*!Local APIC ID: The initial APIC-ID is used to identify the executing logical processor.<br/>
    It can also be identified via the cpuid 0BH leaf ( CPUID.0Bh.EDX[x2APIC-ID] ).<br/>
    Valid: Pentium 4 and subsequent processors.*/
    uint8_t local_apic_id;
} __attribute__((packed)) cpuid_0x01h_ebx_fields;

//! CPU features from ECX register.
typedef struct cpuid_0x01h_ecx_fields
{
    //! Prescott New Instructions-SSE3 (PNI)
    uint8_t sse3 : 1;
    //! PCLMULQDQ
    uint8_t pclmulqdq : 1;
    //! 64-bit debug store (edx bit 21)
    uint8_t dtes64 : 1;
    //! MONITOR and MWAIT instructions (SSE3)
    uint8_t monitor : 1;
    //! CPL qualified debug store
    uint8_t ds_cpl : 1;
    //! Virtual Machine eXtensions
    uint8_t vmx : 1;
    //! Safer Mode Extensions (LaGrande)
    uint8_t smx : 1;
    //! Enhanced SpeedStep
    uint8_t est : 1;

    //! Thermal Monitor 2
    uint8_t tm2 : 1;
    //! Supplemental SSE3 instructions
    uint8_t ssse3 : 1;
    //! L1 Context ID
    uint8_t cnxt_id : 1;
    //! Silicon Debug interface
    uint8_t sdbg : 1;
    //! Fused multiply-add (FMA3)
    uint8_t fma : 1;
    //! CMPXCHG16B instruction
    uint8_t cx16 : 1;
    //! Can disable sending task priority messages
    uint8_t xtpr : 1;
    //! Perfmon & debug capability
    uint8_t pdcm : 1;

    // Reserved
    uint8_t  : 1;
    //! Process context identifiers (CR4 bit 17)
    uint8_t pcid : 1;
    //! Direct cache access for DMA writes
    uint8_t dca : 1;
    //! SSE4.1 instructions 
    uint8_t sse4_1 : 1;
    //! SSE4.2 instructions
    uint8_t sse4_2 : 1;
    //! x2APIC
    uint8_t x2apic : 1;
    //! MOVBE instruction (big-endian)
    uint8_t movbe : 1;
    //! POPCNT instruction
    uint8_t popcnt : 1;

    //! APIC implements one-shot operation using a TSC deadline value
    uint8_t tsc_deadline : 1;
    //! AES instruction set
    uint8_t aes : 1;
    //! XSAVE, XRESTOR, XSETBV, XGETBV
    uint8_t xsave : 1;
    //! XSAVE enabled by OS
    uint8_t osxsave : 1;
    //! Advanced Vector Extensions
    uint8_t avx : 1;
    //! F16C (half-precision) FP feature
    uint8_t f16c : 1;
    //! RDRAND (on-chip random number generator) feature
    uint8_t rdrnd : 1;
    //! Hypervisor present (always zero on physical CPUs)
    uint8_t hypervisor : 1;
} __attribute__((packed)) cpuid_0x01h_ecx_fields;

//! CPU features from EDX register.
typedef struct cpuid_0x01h_edx_fields
{
    //! Onboard x87 FPU
    uint8_t fpu : 1;
    //! Virtual 8086 mode extensions (such as VIF, VIP, PIV)
    uint8_t vme : 1;
    //! Debugging extensions (CR4 bit 3)
    uint8_t de : 1;
    //! Page Size Extension
    uint8_t pse : 1;
    //! Time Stamp Counter
    uint8_t tsc : 1;
    //! Model-specific registers
    uint8_t msr : 1;
    //! Physical Address Extension
    uint8_t pae : 1;
    //! Machine Check Exception
    uint8_t mce : 1;

    //! CMPXCHG8 (compare-and-swap) instruction
    uint8_t cx8 : 1;
    //! Onboard Advanced Programmable Interrupt Controller
    uint8_t apic : 1;
    // Reserved
    uint8_t : 1;
    //! SYSENTER and SYSEXIT instructions
    uint8_t sep : 1;
    //! Memory Type Range Registers
    uint8_t mtrr : 1;
    //! Page Global Enable bit in CR4
    uint8_t pge : 1;
    //! Machine check architecture
    uint8_t mca : 1;
    //! Conditional move and FCMOV instructions
    uint8_t cmov : 1;

    //! Page Attribute Table
    uint8_t pat : 1;
    //! 36-bit page size extension
    uint8_t pse_32: 1;
    //! Processor Serial Number
    uint8_t psn : 1;
    //! CLFLUSH instruction (SSE2)
    uint8_t clfsh : 1;
    // Reserved
    uint8_t : 1;
    //! Debug store: save trace of executed jumps
    uint8_t ds : 1;
    //! Onboard thermal control MSRs for ACPI
    uint8_t acpi : 1;
    //! MMX instructions
    uint8_t mmx : 1;

    //! FXSAVE, FXRESTOR instructions, CR4 bit 9
    uint8_t fxsr : 1;
    //! SSE instructions (a.k.a. Katmai New Instructions)
    uint8_t sse : 1;
    //! SSE2 instructions
    uint8_t sse2 : 1;
    //! CPU cache implements self-snoop
    uint8_t ss : 1;
    //! Hyper-threading
    uint8_t htt : 1;
    //! Thermal monitor automatically limits temperature
    uint8_t tm : 1;
    //! IA64 processor emulating x86
    uint8_t ia64 : 1;
    //! Pending Break Enable (PBE# pin) wakeup capability
    uint8_t pbe : 1;
} __attribute__((packed)) cpuid_0x01h_edx_fields;

//! Processor Info and Feature Bits.
typedef struct cpuid_0x01h_fields
{
    //! EAX fields.
    /*!//! CPU's stepping, model, and family information*/
    cpuid_0x01h_eax_fields eax;
    //! EBX fields.
    /*!//! CPU additional information.*/
    cpuid_0x01h_ebx_fields ebx;
    //! ECX fields.
    /*!//! CPU features from ECX register.*/
    cpuid_0x01h_ecx_fields ecx;
    //! EDX fields.
    /*!//! CPU features from EDX register.*/
    cpuid_0x01h_edx_fields edx;
} cpuid_0x01h_fields;

//! Processor Info and Feature Bits.
typedef union cpuid_0x01h
{
    //! Value of union
    uint32_t value[4];
    //! Separate fields.
    cpuid_0x01h_fields fields;
} cpuid_0x01h;

#endif