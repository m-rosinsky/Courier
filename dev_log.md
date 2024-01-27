# Courier Development Log

# 1/27/2024

I would love to have a working made-from-scratch language as something to show for completing the Compiler Theory course.

About a year and a half ago I was working on my first attempt and an _interpreted_ language, Rosky, which is still on my Github [here](https://github.com/m-rosinsky/Rosky_Lang).

I hope this compiled language captures my growth as a software engineer, and that I can look back on my first interpreted language and see how far I've come.

In my development of Rosky, I didn't use any kind of branching scheme, CI/CD, or even have a test bench for my code. I intend to have all the makings of a production-level project in Courier, which will hopefully be a testament to my growth.

### Development Environment
---

I plan on developing this using Docker to make it as easy as possible to test on Linux.

C++ is notoriously difficult to develop cross-platform code, since Windows very rarely sees eye-to-eye with code that was explicity written for it, but if Docker can help testing code against windows machines, even better.

I'll be setting up Github Actions to perform CI/CD testing, which will help me test cross-platform functionality.

#