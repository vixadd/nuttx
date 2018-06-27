# Fmoto
FM Moto Mods for the Moto Z line of Smartphones.

## NuttX
NuttX is a real timed embedded operating system (RTOS). Its goals are:
### Small Footprint
Usable in all but the tightest micro-controller environments, The focus is on the tiny-to-small, deeply embedded environment.
### Rich Feature OS Set
The goal is to provide implementations of most standard POSIX OS interfaces to support a rich, multi-threaded development environment for deeply embedded processors.
NON-GOALS: It is not a goal to provide the level of OS features like those provided by Linux. In order to work with smaller MCUs, small footprint must be more important than an extensive feature set. But standard compliance is more important than small footprint. Surely a smaller RTOS could be produced by ignoring standards. Think of NuttX is a tiny Linux work-alike with a much reduced feature set.
### Highly Scalable
Fully scalable from tiny (8-bit) to moderate embedded (32-bit). Scalability with rich feature set is accomplished with: Many tiny source files, link from static libraries, highly configurable, use of weak symbols when available.
### Standards Compliance
NuttX strives to achieve a high degree of standards compliance. The primary governing standards are POSIX and ANSI standards. Additional standard APIs from Unix and other common RTOS's are adopted for functionality not available under these standards or for functionality that is not appropriate for the deeply-embedded RTOS (such as fork()).
Because of this standards conformance, software developed under other standard OSs (such as Linux) should port easily to NuttX.
### Real-Time
Fully pre-emptible; fixed priority, round-robin, and "sporadic" scheduling.
### Totally Open
Non-restrictive BSD license.
### GNU Toolchains
Compatible GNU toolchains based on buildroot available for download to provide a complete development environment for many architectures.
