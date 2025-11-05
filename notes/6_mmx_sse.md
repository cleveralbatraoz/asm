# SIMD in x86: MMX, MMX-Ext, 3DNow!, SSE — Complete Lecture Note

This note consolidates **all topics and examples** from the lecture (including board photos).  
Structured as:

- **Part I — Theory & Architecture** (motivation, design, evolution)
- **Part II — Instruction Reference & Practical Examples** (grouped tables + worked code)

---

# Part I — Theory & Architecture

## 1. Why SIMD?

Multimedia and numeric workloads often apply the **same operation** to large arrays:

- Image pixels
- Audio samples
- Video blocks
- Vector math

Naively doing scalar operations is slow. SIMD (Single Instruction, Multiple Data) lets one instruction operate on **multiple elements in parallel** packed in a wide register.

### 1.1 Evolution Overview

| Generation | Vendor | Focus | Register Size | Data Types (initially) |
|-----------|--------|-------|---------------|------------------------|
| MMX       | Intel  | Integer SIMD                   | 64-bit | Integers |
| 3DNow!    | AMD    | Float SIMD on MMX regs         | 64-bit | Floats |
| MMX-Ext   | AMD/Intel | Richer integer SIMD        | 64-bit | Integers |
| SSE       | Intel  | Packed single-precision float  | 128-bit | Floats |
| SSE2+     | Intel/AMD | Double + integers + more    | 128-bit | Floats & Ints |

All of them are responses to the same pressure: **fast media, 3D, DSP, codecs**.

---

## 2. MMX: Design & FPU Register Reuse

**MMX (MultiMedia eXtension)** was introduced with Pentium MMX.

Key design decision: **no new physical register file**.

- x87 FPU has 8×80-bit stack registers `ST0–ST7`.
- MMX defines **8 logical 64-bit registers `MM0–MM7`** as aliases for the **low 64 bits** of these.
- So: same hardware, two views.

| Feature              | Description                          |
|----------------------|--------------------------------------|
| Logical regs         | `MM0–MM7`                            |
| Backed by            | low 64 bits of `ST0–ST7`             |
| Width                | 64 bits                              |
| Data types           | **integers only**                    |
| Addressing model     | Flat registers (not a stack)         |
| FPU interaction      | Shared state → needs `EMMS`          |

This reuse made MMX cheaper in silicon, but introduced constraints.

### 2.1 Packed Views

Each `MMx` is interpreted as:

| View    | Layout                  |
|--------|--------------------------|
| QWORD  | 1 × 64-bit               |
| DWORD  | 2 × 32-bit               |
| WORD   | 4 × 16-bit               |
| BYTE   | 8 × 8-bit                |

Operations are defined **lane-wise**.

---

## 3. “Economy Mode” Instruction Set

MMX (and early SSE integer extensions) are **not fully symmetric**.  
Not every conceptually possible variant exists:

- Only sizes and forms that are **practically useful** were added.
- Some combinations are missing: e.g. saturating variants only for bytes/words; some shifts missing for bytes; no general 64-bit integer add in MMX, etc.

This is deliberate: silicon cost vs realistic use cases.

---

## 4. Fixed-Point Arithmetic & Why Integers

For video & image:

- Final format: usually **8 bits per channel**.
- Intermediate steps: often **16-bit fixed point** is enough.
- Fixed-point integer arithmetic is:
  - Faster than floats (especially then).
  - Accurate enough for visual output.

Hence MMX is integer-only and focuses on:
- 8-bit and 16-bit lanes
- Saturation (no wrapping artifacts)
- Simple parallel ops

---

## 5. FPU + MMX Interaction

Because MMX aliases x87:

- Any MMX operation marks FPU state “busy”.
- To safely use x87 again: **must execute `EMMS`**.

```asm
emms    ; Empty MMX State — clear tag words, FPU usable again
```

If you forget:
- x87 stack appears full/dirty.
- You get NaNs and strange behavior.

**Rule:**  
Use `EMMS` at the **end** of MMX-using functions (not inside hot loops).

(AMD added `FEMMS` for 3DNow! as a faster variant.)

---

## 6. Limitations of MMX

Summarizing:

- No floats.
- Shared with FPU, requires `EMMS/FEMMS`.
- No per-instruction immediates for vector arithmetic (constants via memory or GPR + `MOVD`).
- No division; emulate with multiply + shift (fixed-point).
- Some sizes/variants intentionally missing.

These gaps motivated MMX-Ext, 3DNow!, and SSE.

---

# Part II — Instruction Reference & Practical Examples

## 1. MMX Core Instructions

### 1.1 Data Movement

- `MOVQ` — move 64 bits between `mm`/memory.
- `MOVD` — move 32 bits between `mm` and GPR/memory.
  - Writing to `mm`: low 32 bits from source, upper 32 bits = 0.

### 1.2 Packing (Narrowing with Saturation)

Used to go from higher-precision intermediate results back to 8-bit/16-bit outputs.

- `PACKSSDW` — 32 → 16, signed saturation.
- `PACKSSWB` — 16 → 8, signed saturation.
- `PACKUSWB` — 16 → 8, **unsigned** saturation (common for image write-back).

Rationale: avoid wraparound like 256 → 0, -1 → 255 for pixels.

### 1.3 Unpacking (Interleaving / Widening)

- `PUNPCKLBW`, `PUNPCKHBW`
- `PUNPCKLWD`, `PUNPCKHWD`
- `PUNPCKLDQ`, `PUNPCKHDQ`

They interleave low/high halves of two registers and are used to widen or de-interleave data.

### 1.4 Arithmetic

**Add / Subtract**

- `PADDB/W/D` — normal add.
- `PSUBB/W/D` — normal sub.
- `PADDSB/W` — signed saturating add.
- `PADDUSB/W` — unsigned saturating add.
- `PSUBSB/W` — signed saturating sub.
- `PSUBUSB/W` — unsigned saturating sub.

**Multiply**

- `PMULLW` — low 16 bits × 16 bits → low 16.
- `PMULHW` — high 16 bits of signed product.
- `PMADDWD` — multiply-add pairs, common for dot products.

### 1.5 Shifts

- `PSLLW/D/Q` — logical left.
- `PSRLW/D/Q` — logical right.
- `PSRAW/D` — arithmetic right (signed).

### 1.6 Logical

- `PAND`, `POR`, `PXOR`, `PANDN` — standard logic ops.
  - `PANDN` = `(~dst) & src`

### 1.7 Comparisons

- `PCMPEQB/W/D` — equal.
- `PCMPGTB/W/D` — greater-than (signed).

Result per lane: `0xFF..FF` if true, else `0x00`.

---

## 2. MMX-Ext (AMD) and Related Integer Extensions

Adds missing operations:

`PAVGB/W`, `PMAXUB/SW`, `PMINUB/SW`, `PEXTRW`, `PINSRW`, `PMULHUW`, `PMOVMSKB`, `PSHUFW`, `PSADBW`

`PSADBW` (Sum of Absolute Differences) became core for motion estimation and image comparison.

---

## 3. 3DNow! (AMD)

- Built on MMX regs.
- Adds packed float arithmetic, reciprocals, prefetch instructions.
- Replaced by SSE but relevant historically.
- `FEMMS` clears MMX/FPU faster.

---

## 4. Memory & Cache: Streaming and Prefetch

- `MOVNTQ` — non-temporal store (bypass cache).
- `SFENCE` — ensures visibility of streaming writes.
- `MASKMOVQ` — masked store via `EDI`.
- `PREFETCHT0/T1/T2/NTA` — prefetch levels (temporal/non-temporal).

---

## 5. SSE — Streaming SIMD Extensions

| Feature | Description |
|----------|--------------|
| Registers | XMM0–XMM7 (32-bit) / XMM0–XMM15 (64-bit) |
| Width | 128 bits |
| Independent from FPU | Yes |
| Base data type | 4×32-bit single floats |

Instructions: `ADDPS/SS`, `SUBPS/SS`, `MULPS/SS`, `DIVPS/SS`, `SQRTPS/SS`

SSE2 adds double-precision and integers.

### 5.1 Memory Movement: `MOVAPS` vs `MOVUPS`

| Instruction | Description |
|--------------|--------------|
| `MOVAPS` | Move **aligned** packed singles (16-byte aligned). Misaligned → #GP fault. |
| `MOVUPS` | Move **unaligned** packed singles (no alignment needed). |

**Usage:**  
- Early CPUs: `MOVUPS` slower if misaligned.  
- Modern CPUs: performs ≈ `MOVAPS`.  
- Lecturer’s note: “**MOVUPS is totally fine nowadays.**”  
- Use `MOVAPS` only if you’re sure of 16-byte alignment.  

### 5.2 `SS` vs `PS` Suffixes

| Suffix | Meaning | Operates On |
|---------|----------|--------------|
| `PS` | Packed Single-Precision | All 4 floats in XMM |
| `SS` | Scalar Single-Precision | **Only lowest** 32-bit float (others unchanged) |

`addss xmm0, xmm1` → affects only low float, upper lanes preserved.  
`addps xmm0, xmm1` → adds all 4 lanes.

Use:
- `PS` for full vector math.
- `SS` for scalar or mixed operations.

---

## 6. Worked Examples

### 6.1 Max of Two Arrays — Signed Bytes

```asm
loop_signed_max:
    movq mm0, [A]
    movq mm1, [B]
    movq mm2, mm0
    pcmpgtb mm2, mm1
    pand mm3, mm2, mm0
    pandn mm2, mm2, mm1
    por mm3, mm2
    movq [C], mm3
    add A, 8
    add B, 8
    add C, 8
    sub N, 8
    jg loop_signed_max
    emms
```

### 6.2 Max of Two Arrays — Unsigned Bytes

```asm
loop_unsigned_max:
    movq mm0, [A]
    movq mm1, [B]
    movq mm2, mm0
    psubusb mm2, mm1
    paddb mm2, mm1
    movq [C], mm2
    add A, 8
    add B, 8
    add C, 8
    sub N, 8
    jg loop_unsigned_max
    emms
```

### 6.3 Max of Unsigned 32-bit Integers

```asm
movq mm2, mm0
movq mm3, mm1
pxor mm2, mm7
pxor mm3, mm7
movq mm4, mm2
pcmpgtd mm4, mm3
pand mm5, mm4, mm0
pandn mm4, mm4, mm1
por mm5, mm4
```

### 6.4 PSADBW in Motion Estimation

```asm
movq mm0, [blockA]
movq mm1, [blockB]
psadbw mm0, mm1
paddd mm_total, mm0
```

---

# References

- Intel® 64 and IA‑32 Architectures Software Developer’s Manual, Vol. 2  
- AMD 3DNow!™ Technology Manual  
- Agner Fog — *Optimizing Assembly for Modern x86 CPUs*  
