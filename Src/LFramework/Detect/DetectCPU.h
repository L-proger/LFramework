#ifndef DetectCPU_h__
#define DetectCPU_h__

#define LF_CPU_ARCH_ALPHA 0
#define LF_CPU_ARCH_AMD64 1
#define LF_CPU_ARCH_ARM 2
#define LF_CPU_ARCH_ARM64 3
#define LF_CPU_ARCH_BLACKFIN 4
#define LF_CPU_ARCH_CONVEX 5
#define LF_CPU_ARCH_IX86 6
#define LF_CPU_ARCH_IA64 7
#define LF_CPU_ARCH_MOTOROLA68K 8
#define LF_CPU_ARCH_MIPS 9
#define LF_CPU_ARCH_POWERPC 10
#define LF_CPU_ARCH_SPARC 11


#ifndef LF_CPU_ARCH

#if defined(__alpha__) || defined(__alpha) || defined(_M_ALPHA)
	#define LF_CPU_ARCH LF_CPU_ARCH_ALPHA
#elif defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
	#define LF_CPU_ARCH LF_CPU_ARCH_AMD64
#elif defined(__arm__) || defined(__thumb__) || defined(__TARGET_ARCH_ARM) || defined(__TARGET_ARCH_THUMB) || defined(_ARM) || defined(_M_ARM)|| defined(_M_ARMT) || defined(__arm)
	#define LF_CPU_ARCH LF_CPU_ARCH_ARM
#elif defined(__aarch64__)
	#define LF_CPU_ARCH LF_CPU_ARCH_ARM64
#elif defined(__bfin) || defined(__BFIN__)
	#define LF_CPU_ARCH LF_CPU_ARCH_BLACKFIN
#elif defined(__convex__)
	#define LF_CPU_ARCH LF_CPU_ARCH_CONVEX

#elif defined(i386) || defined(__i386) || defined(__i386__) || defined(__IA32__) \
|| defined(_M_I86) || defined(_M_IX86) || defined(__X86__) || defined(_X86_) \
|| defined(__THW_INTEL__) || defined(__I86__) || defined(__INTEL__) || defined(__386)
	#define LF_CPU_ARCH LF_CPU_ARCH_IX86

#elif defined(__ia64__) || defined(_IA64) || defined(__IA64__) || defined(__ia64) \
|| defined(_M_IA64) || defined(_M_IA64) || defined(__itanium__) 
	#define LF_CPU_ARCH LF_CPU_ARCH_IA64

#elif defined(__m68k__) || defined(M68000) || defined(__MC68K__) 
	#define LF_CPU_ARCH LF_CPU_ARCH_MOTOROLA68K

#elif defined(__mips__) || defined(mips) || defined(__mips)  || defined(__MIPS__)
	#define LF_CPU_ARCH LF_CPU_ARCH_MIPS
	
#elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__)  || defined(__POWERPC__) \
|| defined(__ppc__) || defined(__ppc64__) || defined(__PPC__) || defined(__PPC64__) \
|| defined(_ARCH_PPC) || defined(_ARCH_PPC64) || defined(_M_PPC) || defined(__ppc)
	#define LF_CPU_ARCH LF_CPU_ARCH_POWERPC
	
#elif defined(__sparc__) || defined(__sparc)
	#define LF_CPU_ARCH LF_CPU_ARCH_MIPS

#else
#error "Unknown CPU arch" 
#endif

#endif
 

namespace LFramework {

	enum class CpuArchitecture {
		Alpha = LF_CPU_ARCH_ALPHA,
		AMD64 = LF_CPU_ARCH_AMD64,
		ARM = LF_CPU_ARCH_ARM,
		ARM64 = LF_CPU_ARCH_ARM64,
		Blackfin = LF_CPU_ARCH_BLACKFIN,
		Convex = LF_CPU_ARCH_CONVEX,
		IX86 = LF_CPU_ARCH_IX86,
		IA64 = LF_CPU_ARCH_IA64,
		Motorola68K = LF_CPU_ARCH_MOTOROLA68K,
		MIPS = LF_CPU_ARCH_MIPS,
		PowerPC = LF_CPU_ARCH_POWERPC,
		SPARC = LF_CPU_ARCH_SPARC
	};

	namespace Detect {
		static constexpr CpuArchitecture cpuArchitecture() {
			return static_cast<CpuArchitecture>(LF_CPU_ARCH);
		}
	}
}

#endif // DetectCPU_h__
