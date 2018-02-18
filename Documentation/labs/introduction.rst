============
Introduction
============

Lab objectives
==============

* presenting the rules and objectives of the Operating Systems 2 lab
* introducing the lab documentation
* introducing the Linux kernel and related resources

Keywords
========

*  kernel, kernel programming
*  Linux, vanilla, http://www.kernel.org
*  cscope, LXR
*  gdb, /proc/kcore, addr2line, dump\_stack

About this laboratory
=====================

The Operating Systems 2 lab is a kernel programming and driver development lab. 
The objectives of the laboratory are:

* deepening the notions presented in the course
* presentation of kernel programming interfaces (kernel API)
* gaining documenting, development and debugging skills on a freestanding 
  environment
* acquiring knowledge and skills for drivers development

A laboratory will present a set of concepts, applications and commands
specific to a given problem. The lab will start with a presentation
(each lab will have a set of slides) (15 minutes) and the remaining
time will be allocated to the lab exercises (80 minutes).

For best laboratory performance, we recommend that you read the related slides. 
To fully understand a laboratory, we recommend going through the lab support. For
in-depth study, use the supporting documentation.

Documentation
=============

-  Linux

   -  `Linux Kernel Development, 3rd
      Edition <http://www.amazon.com/Linux-Kernel-Development-Robert-Love/dp/0672329468/>`__
   -  `Linux Device Drivers, 3rd
      Edition <http://free-electrons.com/doc/books/ldd3.pdf>`__
   -  `Essential Linux Device
      Drivers <http://www.amazon.com/Essential-Device-Drivers-Sreekrishnan-Venkateswaran/dp/0132396556>`__

-  General

   -  `mailing list <http://cursuri.cs.pub.ro/cgi-bin/mailman/listinfo/pso>`__
      (`searching the mailing list <http://blog.gmane.org/gmane.education.region.romania.operating-systems-design>`__)

Source code navigation
======================

cscope
------

`Cscope <http://cscope.sourceforge.net/>`__ is a tool for
efficient navigation of C sources. To use it, a cscope database must 
be geberated from the existing sources. In a Linux tree, the command
:command:`make ARCH = x86 cscope` is sufficient. Specification of the 
architecture through the ARCH variable is optional but recommended; 
otherwise, some architecture dependent functions will appear multiple 
times in the database.

Cscope can also be used as stand-alone, but it is more useful when 
combined with an editor. To use cscope with Vim, it is necessary to
install both packages and add the following lines to the file
:file:`.vimrc` (the machine in the lab already has the settings):

.. code-block:: vim

    if has("cscope")
            " Look for a 'cscope.out' file starting from the current directory,
            " going up to the root directory.
            let s:dirs = split(getcwd(), "/")
            while s:dirs != []
                    let s:path = "/" . join(s:dirs, "/")
                    if (filereadable(s:path . "/cscope.out"))
                            execute "cs add " . s:path . "/cscope.out " . s:path . " -v"
                            break
                    endif
                    let s:dirs = s:dirs[:-2]
            endwhile

            set csto=0  " Use cscope first, then ctags
            set cst     " Only search cscope
            set csverb  " Make cs verbose

            nmap `<C-\>`s :cs find s `<C-R>`=expand("`<cword>`")`<CR>``<CR>`
            nmap `<C-\>`g :cs find g `<C-R>`=expand("`<cword>`")`<CR>``<CR>`
            nmap `<C-\>`c :cs find c `<C-R>`=expand("`<cword>`")`<CR>``<CR>`
            nmap `<C-\>`t :cs find t `<C-R>`=expand("`<cword>`")`<CR>``<CR>`
            nmap `<C-\>`e :cs find e `<C-R>`=expand("`<cword>`")`<CR>``<CR>`
            nmap `<C-\>`f :cs find f `<C-R>`=expand("`<cfile>`")`<CR>``<CR>`
            nmap `<C-\>`i :cs find i ^`<C-R>`=expand("`<cfile>`")`<CR>`$`<CR>`
            nmap `<C-\>`d :cs find d `<C-R>`=expand("`<cword>`")`<CR>``<CR>`

            " Open a quickfix window for the following queries.
            set cscopequickfix=s-,c-,d-,i-,t-,e-,g-
    endif

The script searches for a file called :file:`cscope.out` in the current directory, or
in parent directories. If Vim finds this file, you can use the shortcut :code:`Ctrl +]`
or :code:`Ctrl+\ g` (the combination control-\\ followed by g) to jump directly to 
the definition of the word under the cursor (function, variable, structure, etc.). 
Similarly, you can use :code:`Ctrl+\ s` to go where the word under the cursor is used.

You can take a cscope-enabled '.vimrc' file (also contains other goodies) from
https://github.com/ddvlad/cfg/blob/master/\_vimrc.
The following guidelines are based on this file, but also show basic vim commands 
that have the same effect.

If there are more than one results (usually there are) you can move between them
using :code:`F6` and :code:`F5` (:code:`:ccnext`  and :code:`:cprev`).
You can also open a new panel showing the results using :code:`:copen`. To close
the panel, use the :code:`:cclose` command.

To return to the previous location, use :code:`Ctrl+o` (o, not zero).
The command can be used multiple times and works even if cscope changed the
file you are currently editing.

To go to a symbol definition directly when vim starts, use :code:`vim -t <symbol_name>`
(for example :code:`vim -t task_struct`). Otherwise, if you started vim and want
to search for a symbol by name, use :code:`cs find g <symbol_name>` (for example
:code:`cs find g task_struct`).

If you fond more than one results and opened a panel showing all the matches
(using :code:`:copen`) and you want to find a symbol of type structure,
it is recommended to search in the results panel (using :code:`/` -- slash)
the character :code:`{` (opening brace).

.. important::
    You can get a summary of all the :command:`cscope` commands using :command:`:cs help`.

    For more info, use the Vim built-in help command: :command:`:h cscope` or :command:`:h copen`.

If you use :command:`emacs`, install the :code:`xcscope-el` package and
add the following lines in :file:`~/.emacs`.

.. code-block:: vim

    (require ‘xcscope)
    (cscope-setup)

These commands will activate cscope for the C and C++ modes automatically.
:code:`C-s s` is the key bindings prefix and :code:`C-s s s` is used to
search for a symbol (if you call it when the cursor is over a word,
it will use that). For more details, check `https://github.com/dkogan/xcscope.el`

Kscope
~~~~~~

For a simpler interface, `Kscope <http://sourceforge.net/projects/kscope/>`__ 
is a cscope frontend which uses QT. It is lightweight, very fast and very
easy to use. It allows searching using regular expressions, call graphs, etc.
Kscope is no longer mantained.

There is also a `port <https///opendesktop.org/content/show.php/Kscope4?content=156987>`__
of version 1.6 for Qt4 and KDE 4 which keeps the integration of the text
editor Kate and is easier to use than the last version on SourceForge.

LXR Cross-Reference
-------------------

LXR (LXR Cross-Reference) is a tool that allows indexing and
referencing the symbols in the source code of a program using
a web interface. The web interface shows links to
locations in files where a symbol is defined or used. Development website
for LXR is http://sourceforge.net/projects/lxr. Similar tools
are `OpenGrok <http://www.opensolaris.org/os/project/opengrok/>`__ and
`Gonzui <http://en.wikipedia.org/wiki/Gonzui>`__.

Although LXR was originally intended for the Linux kernel sources, it is
also used in the sources of `Mozilla <http://lxr.mozilla.org/>`__, 
`Apache HTTP Server <http://apache.wirebrain.de/lxr/>`__ and
`FreeBSD <http://lxr.linux.no/freebsd/source>`__.

There are a number of sites that use LXR for cross-referencing the
the sources of the Linux kernel, the main site being `the original site of
development <http://lxr.linux.no/linux/>`__ which does not work anymore. You can
use `https://elixir.bootlin.com/ <https://elixir.bootlin.com/>`__.

LXR allows searching for an identifier (symbol), after a free text
or after a file name. The main feature and, at the same
time, the main advantage provided is the ease of finding the declaration
of any global identifier. This way, it facilitates quick access to function 
declarations, variables, macro definitions and the code can be easily 
navigated. Also, the fact that it can detect what code areas are affected 
when a variable or function is changed is a real advantage in the development 
and debugging phase.

SourceWeb
---------

`SourceWeb <http://rprichard.github.io/sourceweb/>`__ is a source code indexer
for C and C++. It uses the 
`framework <http://clang.llvm.org/docs/IntroductionToTheClangAST.html>`__
provided by the Clang compiler to index the code.

The main difference between cscope and SourceWeb is the fact that SourceWeb
is, in a way, a compiper pass. SourceWeb doesn't index all the code, but
only the coe that was efectively compiled by the compiler. This way, some
problems are eliminated, such as ambiguities about which variant of a function
defined in multiple places is used. This also means that the indexing takes
more time, because the compiled files must pass one more time through
the indexer to generate the references.

Usage example:

.. code-block:: bash

    make oldconfig
    sw-btrace make -j4
    sw-btrace-to-compile-db
    sw-clang-indexer --index-project
    sourceweb index

:file:`sw-btrace` is a scrit that adds the :file:`libsw-btrace.so`
library to :code:`LD_PRELOAD`. This way, the library is loaded by
every process started by :code:`make` (basically, the compiler),
registers the commands used to start the processes and generates
a filed called :file:`btrace.log`. This file is then used by 
:code:`sw-btrace-to-compile-db` which converts it to a format defined
by clang: `JSON Compilation Database <http://clang.llvm.org/docs/JSONCompilationDatabase.html>`__.
This JSON Compilation Database resulted from the above steps is then
used by the indexer, which makes one more pass through the compiled
source files and generates the index used by the GUI.

Word of advice: don't index the sources you are working with, but use
a copy, because SourceWeb doesn't have, at this moment, the capability
to regenerate the index for a single file and you will have to regenerate
the complete index.

Debugging
=========

Debugging a kernel is a much more difficult process than the debugging
of a program, because there is no support from the operating system.
This is why this process is usually done using two computers, connected
on serial interfaces.

gdb (Linux)
-----------

A simpler debug method on Linux, but with many disadvantages,
is local debugging, using `gdb <http://www.gnu.org/software/gdb/>`__,
the uncompressed kernel image (:file:`vmlinux`) and :file:`/proc/kcore`
(the real-time kernel image). This method is usually used to inspect
the kernel and detect certain inconsistencies while it runs. The
method is useful especially if the kernel was compiled using the
:code:`-g` option, which keeps debug information. Some well-known 
debug techniques can't be used by this method, such as breakpoints
of data modification.

The uncompressed kernel image offers information about the data structures
and symbols it contains.

.. code-block:: bash

    so2@spook$ cd /usr/src/linux
    so2@spook$ file vmlinux
    vmlinux: ELF 32-bit LSB executable, Intel 80386, ...
    so2@spook$ nm vmlinux | grep sys_call_table
    c02e535c R sys_call_table
    so2@spook$ cat System.map | grep sys_call_table
    c02e535c R sys_call_table

The :command:`nm` utility is used to show the symbols in an object or
executable file. In our case, :file:`vmlinux` is an ELF file. Alternately,
we can use the file :file:`System.map` to view information about the
symbols in kernel.

Then we use :command:`gdb` to inspect the symbols using the uncompressed
kernel image. A simple :command:`gdb` session is the following:

.. code-block:: bash

    so2@spook$ cd /usr/src/linux
    so2@spook$ gdb --quiet vmlinux
    Using host libthread_db library "/lib/tls/libthread_db.so.1".
    (gdb) x/x 0xc02e535c
    0xc02e535c `<sys_call_table>`:    0xc011bc58
    (gdb) x/16 0xc02e535c
    0xc02e535c `<sys_call_table>`:    0xc011bc58      0xc011482a      0xc01013d3     0xc014363d
    0xc02e536c `<sys_call_table+16>`: 0xc014369f      0xc0142d4e      0xc0142de5     0xc011548b
    0xc02e537c `<sys_call_table+32>`: 0xc0142d7d      0xc01507a1      0xc015042c     0xc0101431
    0xc02e538c `<sys_call_table+48>`: 0xc014249e      0xc0115c6c      0xc014fee7     0xc0142725
    (gdb) x/x sys_call_table
    0xc011bc58 `<sys_restart_syscall>`:       0xffe000ba
    (gdb) x/x &sys_call_table
    0xc02e535c `<sys_call_table>`:    0xc011bc58
    (gdb) x/16 &sys_call_table
    0xc02e535c `<sys_call_table>`:    0xc011bc58      0xc011482a      0xc01013d3     0xc014363d
    0xc02e536c `<sys_call_table+16>`: 0xc014369f      0xc0142d4e      0xc0142de5     0xc011548b
    0xc02e537c `<sys_call_table+32>`: 0xc0142d7d      0xc01507a1      0xc015042c     0xc0101431
    0xc02e538c `<sys_call_table+48>`: 0xc014249e      0xc0115c6c      0xc014fee7     0xc0142725
    (gdb) x/x sys_fork
    0xc01013d3 `<sys_fork>`:  0x3824548b
    (gdb) disass sys_fork
    Dump of assembler code for function sys_fork:
    0xc01013d3 `<sys_fork+0>`:        mov    0x38(%esp),%edx
    0xc01013d7 `<sys_fork+4>`:        mov    $0x11,%eax
    0xc01013dc `<sys_fork+9>`:        push   $0x0
    0xc01013de `<sys_fork+11>`:       push   $0x0
    0xc01013e0 `<sys_fork+13>`:       push   $0x0
    0xc01013e2 `<sys_fork+15>`:       lea    0x10(%esp),%ecx
    0xc01013e6 `<sys_fork+19>`:       call   0xc0111aab `<do_fork>`
    0xc01013eb `<sys_fork+24>`:       add    $0xc,%esp
    0xc01013ee `<sys_fork+27>`:       ret
    End of assembler dump.

It can be noticed that the uncompressed kernel image was used as an argument
for :command:`gdb`. The image can be found in the root of the kernel sources
after compilation.

A few commands used for debugging using :command:`gdb` are:

- :command:`x` (examine) -- used to show the contents of the memory area
  whose address is specified as an argument to the command (this address
  can be the value of a physical address, a symbol or the address of a
  symbol). It can take as arguments (preceded by :code:`/`): the format
  to display the data in (:code:`x` for hexadecimal, :code:`d` for
  decimal, etc), how many memory units to display and the size of a
  memory unit.

- :command:`disassemble` - Used to disassemble a function.

- :command:`p` (print) - Used to evaluate and show the value of an
  expression. The format to show the data in can be specified as
  an argument (:code:`/x` for hexadecimal, :code:`/d` for decimal, etc.).

The analysis of the kernel image is a method of static analysis. If we
want to perform dynamic analysis (analyzing how the kernel runs, not
only its static image) we can use :file:`/proc/kcore`; this is a dynamic
image (in memory) of the kernel.

.. code-block:: bash

    so2@spook$ gdb /usr/src/linux/vmlinux /proc/kcore
    Core was generated by `root=/dev/hda3 ro'.
    #0  0x00000000 in ?? ()
    (gdb) p sys_call_table
    $1 = -1072579496
    (gdb) p /x sys_call_table
    $2 = 0xc011bc58
    (gdb) p /x &sys_call_table
    $3 = 0xc02e535c
    (gdb) x/16 &sys_call_table
    0xc02e535c `<sys_call_table>`:    0xc011bc58      0xc011482a      0xc01013d3     0xc014363d
    0xc02e536c `<sys_call_table+16>`: 0xc014369f      0xc0142d4e      0xc0142de5     0xc011548b
    0xc02e537c `<sys_call_table+32>`: 0xc0142d7d      0xc01507a1      0xc015042c     0xc0101431
    0xc02e538c `<sys_call_table+48>`: 0xc014249e      0xc0115c6c      0xc014fee7     0xc0142725

Using the dynamic image of the kernel is useful for detecting `rootkits <http://en.wikipedia.org/wiki/Rootkit>`__.

- `Linux Device Drivers 3rd Edition - Debuggers and Related Tools <http://linuxdriver.co.il/ldd3/linuxdrive3-CHP-4-SECT-6.html>`__
- `Detecting Rootkits and Kernel-level Compromises in Linux <http://www.securityfocus.com/infocus/1811>`__
- `User-Mode Linux <http://user-mode-linux.sf.net/>`__

Getting a stack trace
---------------------

Somethimes, you will want information about the trace the execution
reaches a certain point. You can determine this information using 
:command:`cscope` or LXR, but some function are called from many
execution paths, which makes this method difficult.

In these situations, it is useful to get a stack trace, which can be
simply done using the function :code:`dump_stack()`.

Documentation
=============

Kernel development is a difficult process, compared to user space
programming. The API is different and the complexity of the subsystems
in kernel requires additional preparation. The associated documentation
is heterogeneous, sometimes requiring the inspection of multiple sources
to have a more complete understanding of a certain aspect.

The main advantages of the Linux kernel are the access to sources and
the open development system. Because of this, the Internet offers a
larger number of documentation for kernel.

A few links related to the Linux kernel are shown bellow:

- `KernelNewbies <http://kernelnewbies.org>`__
- `KernelNewbies - Kernel Hacking <http://kernelnewbies.org/KernelHacking>`__
- `Kernel Analysis - HOWTO <http://www.tldp.org/HOWTO/KernelAnalysis-HOWTO.html>`__
- `Linux Kernel Programming <http://web.archive.org/web/20090228191439/http://www.linuxhq.com/lkprogram.html>`__
- `Linux kernel - Wikibooks <http://en.wikibooks.org/wiki/Linux_kernel>`__

The links are not comprehensive. Using  `The Internet <http://www.google.com>`__ and
`surselor <http://lxr.free-electrons.com/>`__ is essential.

Exercices
=========

Remarks
-------

.. note::

  -  Usually, the steps used to develop a kernel module are the
     following:
  
     -  editing the module source code (on the physical machine);
     -  module compilation (on the physical machine)
     -  generation of the minimal image for the virtual machine.
        This image contains the kernel, your module, busybox and
        eventually test programs.
     -  starting the virtual machine using qemu.
     -  running the tests in the virtual machine.
  
  -  When using cscope, use :file:`~/src/linux`.
     If there is no :file:`cscope.out` file, you can generate it using
     the command :command:`make ARCH=x86 cscope`.

  -  You cand find more details about the virtual machine at 
     :ref:`vm_link`.

.. important::
    Before solving an exercice, **carefully** read all its bullets.

1. Booting the virtual machine
------------------------------

A summary of the virtual machine infrastructure:

-  :file:`~/src/linux` - Linux kernel sources, needed to
   compile modules. The directory contains the file :file:`cscope.out`,
   used for navigation in the source tree.

-  :file:`~/src/linux/tools/labs/qemu`- scripts and auxiliary
   files used to generate and run the qemu VM.

To start the VM, run :command:`QEMU_DISPLAY=sdl make boot` in the directory :file:`~/src/linux/tools/labs`:

.. code-block:: shell

    student@eg106:~$ cd ~/src/linux/tools/labs
    student@eg106:~/src/linux/tools/labs$ QEMU_DISPLAY=sdl make boot

.. note::
    To access the virtual machine, at the login prompt, enter the 
    username :code:`root`; there is no need to enter a password.
    The virtual machine will start with the permissions of the
    root account.

2. Adăugarea și utilizarea unui disc virtual
--------------------------------------------

.. note:: 
    If you don't have the file :file:`mydisk.img`, you can download
    it from the address http://elf.cs.pub.ro/so2/res/laboratoare/mydisk.img.

In the directory :file:`~/src/linux/tools/labs/qemu` you have a new virtual
machine disk, in the file :file:`mydisk.img`. We want to add the disk
to the virtual machine and used while it is running.

Editați fișierul ''Makefile'' pentru a adăuga, la target-ul ''run'',
parametrul ''-drive file=mydisk.img,format=raw''. Rulați ''make'' pentru
a boota mașina virtuală.

În cadrul mașinii virtuale configurați accesul la disc.

``<note tip>`` Nu trebuie să creați manual intrarea aferentă noului disc
în ''/dev'' pentru că mașina virtuală folosește devtmpfs. ``</note>``

Creați directorul ''/test'' și încercați să montați noul
disc:\ ``<code>`` mkdir /test mount /dev/sda /test ``</code>``

Motivul pentru care nu putem monta discul este pentru că nu avem suport
în kernel pentru sistemul de fișiere cu care este formatat discul
''mydisk.img''. Va trebui să identificați sistemul de fișiere aferent
discului ''mydisk.img'' și să compilați suport în kernel pentru acel
sistem de fișiere.

Închideți mașina virtuală (închideți fereastra qemu, nu e nevoie să
folosiți altă comandă). Folosiți comanda ''file'' pe mașina fizică
pentru a afla cu ce sistem de fișiere este formatat fișierul
''mydisk.img''. Veți identifica sistemul de fișiere ''btrfs''.

Va trebui să activați suportul de ''btrfs'' în kernel și să îl
recompilați.

``<note warning>`` Dacă în momentul executării comenzii ''make
menuconfig'' vă apare eroare, este probabil că nu aveți instalat
pachetul ''libncurses5-dev''. Instalați-l folosind comanda

::

    sudo apt-get install libncurses5-dev

``</note>``

``<note tip>`` Intrați în subdirectorul ''linux-4.9.11/''. Executați
''make menuconfig'' și intrați în secțiunea *File systems*. Activați
opțiunea *Btrfs file system support*. Va trebui să folosiți opțiune de
tip ''builtin'' (**nu** modul) adică trebuie să apară ``<*>`` în dreptul
opțiunii (**nu** ``<M>``).

Salvați configurația realizată. Folosiți fișierul de configurare
implicit ''.config''.

În subdirectorul cu sursele nucleului (''linux-4.9.11/'') recompilați
folosind comanda

::

    make

Pentru a aștepta mai puțin, puteți utiliza opțiunea ''-j'' pentru a
folosi mai multe job-uri în paralel. În general se recomandă un număr de
procese cu 1 mai mare decât numărul procesoarelor:

::

    make -j5

``</note>``

După ce se încheie recompilarea, **reporniți** mașina virtuală QEMU:
adică lansați comanda ''make'' în subdirectorul ''qemu-so2/''. Nu este
nevoie să copiați nimic, pentru că fișierul ''bzImage'' este symlink
către imaginea kernel-ului pe care tocmai l-ați recompilat.

În cadrul mașinii virtuale QEMU repetați operațiunile de ''mkdir'' și
''mount''. Având suport pentru sistemul de fișiere btrfs, acum mount se
va termina cu succes.

``<note>`` În elaborarea temelor nu este necesar să recompilați
kernel-ul, veți folosi doar module. Totuși, este important să fiți
familiari cu configurarea și recompilarea unui kernel.

Dacă totuși aveți de gând să recompilați kernel-ul, faceți un back-up
fișierului ''bzImage'' (urmați link-ul din ''qemu-so2'' pentru calea
completă). Astfel veți putea reveni la setup-ul inițial pentru a avea un
mediu identic cu vmchecker. ``</note>``

3. GDB și qemu
--------------

Putem investiga în timp real și depana mașina virtuală QEMU.

Pentru aceasta pornim, în primă fază, mașina virtuală QEMU. Apoi, ne
putem conecta cu ''gdb'' la un mașina vituală QEMU **aflată în rulare**,
folosind comanda

::

    :::bash
    make gdb

Am folosit comanda ''qemu'' cu parametrul ''-s'', ceea ce înseamnă că
ascultă pe portul 1234 de la GDB. Putem face debugging folosind un
*target remote* pentru GDB. Makefile-ul existent are grijă de detalii.

Când atașați un debugger unui proces, procesul este suspendat. Puteți
pune breakpoints și inspecta starea curentă a procesului.

Atașați-vă la mașina virtuală ''qemu'' (folosind comanda ''make gdb'')
și puneți un breakpoint în funcția ''sys\_access'' folosind în consola
GDB comanda

::

    break sys_access

În acest moment mașina virtuală este suspendată. Pentru a continua
execuția ei (până la eventualul apel al funcției ''sys\_access'')
folosiți, în consola GDB comanda\ ``<code>`` continue ``</code>``

În acest moment mașina virtuală este activă și are consola utilizabilă.
Pentru a genera un apel de ''sys\_access'', lansați o comandă ''ls''.
Observați că mașina virtuală a fost din nou suspendată de GDB și a
apărut mesajul aferent de apel al ''sys\_access'' în cadrul GDB.

Urmăriți execuția codului, folosind ''step instruction'', ''continue''
sau ''next instruction''. Probabil nu o să înțelegeți tot ce se
întâmplă, utilizați comenzi precum ''list'' și ''backtrace'' pentru a
urmări logic execuția.

``<hidden>`` RD: Nu știu dacă ar trebui sau nu să se poată vedea codul
sursă. Pe moment dă eroare că nu se vede. O fi ceva de la symlink-uri,
nu m-am prins. ``</hidden>``

``<note tip>`` La prompt-ul ''gdb'', puteți apăsa ''ENTER'' (fără
altceva) pentru a rula ultima comandă încă o dată. ``</note>``

4. GDB spelunking
-----------------

Folosiți ''GDB'' pentru a afișa codul sursă al funcției care creează
thread-uri de kernel (''kernel\_thread'').

``<note tip>`` Puteți folosi GDB pentru analiza statică a imaginii de
kernel folosind, în directorul cu sursele kernel-ului, o comandă de
forma

::

    gdb vmlinux

Parcurgeți secțiunea `gdb (Linux) <#gdb%20(Linux)>`__ din laborator.
``</note>``

Folosiți ''GDB'' pentru a afla adresa variabilei ''jiffies'' în memorie
și conținutul acesteia. Variabila ''jiffies'' reține numărul de tick-uri
(bătăi de ceas) de la pornirea sistemului,.

``<note tip>`` Pentru a urmări valoarea variabilei ''jiffies'' folosiți
analiza dinamică în GDB folosind comanda

::

    make gdb
    `</code>` la fel ca la [[#gdb_si_qemu|exercițiul anterior]].

    Parcurgeți secțiunea [[#gdb (Linux)|gdb (Linux)]] din laborator.
    `</note>`

    `<note tip>`
    Variabila ''jiffies'' este pe 64 de biți. Puteți observa că adresa sa este identică cu cea a variabilei ''jiffies_64''.

    Ca să explorați conținutul unei variabile pe 64 de biți, folosiți, în GDB, construcția
    `<code>`
    x/gx &jiffies

Dacă doreați să afișați conținutul variabilei pe 32 de biți, ați folosi,
în GDB, construcția

::

    x/wx &jiffies

``</note>`` ### 5. cscope spelunking

Folosiți LXR sau cscope în directorul ''/usr/src/linux-so2/'' pentru a
determina locul de definire al unor structuri sau funcții.

Fișierele index cscope sunt deja generate. Folosiți direct Vim și
comenzile aferente pentru parcurgerea codului sursă. De exemplu,
folosiți comanda

::

    vim

pentru a deschide editorul Vim. Apoi, în cadrul editorului, folosiți
comenzi precum '':cs find g task\_struct''.

Determinați fișierul în care sunt definite următoarele tipuri de date:

-  ''struct task\_struct''

-  ''struct semaphore''

-  ''struct list\_head''

-  ''spinlock\_t''

-  ''struct file\_system\_type''

``<note tip>`` Pentru o structură se caută doar numele ei. Spre exemplu,
în cazul ''struct task\_struct'' se caută șirul ''task\_struct''.

De obicei veți obține mai multe match-uri caz în care: 1. Listați toate
match-urile folosind, în Vim, comanda '':copen''. Vă apare o fereastră
secundară cu toate match-urile. 2. Căutați match-ul potrivit (în care
este definită structura) căutând după acoladă deschisă (''{''), un
caracter sigur pe linia de definire a structurii. Pentru căutarea
acoladei deschise folosiți, în Vim, construcția ''/{''. 3. Pe linia
aferentă apăsați ''Enter'' ca să vă ajungă editorul în codul sursă unde
e definită varibila. 4. Închideți fereastra secundară folosind coamanda
'':cclose''. ``</note>``

Determinați fișierul în care sunt declarate următoarele variabile
globale la nivelul nucleului:

-  ''sys\_call\_table''

-  ''file\_systems''

-  ''current''

-  ''chrdevs''

``<note tip>`` Pentru aceasta folosiți în Vim o comandă de forma '':cs f
g ``<symbol>`` (unde construcția ``<symbol>`` reprezintă numele
simbolului căutat). ``</note>``

Determinați fișierul în care sunt declarate următoarele funcții:

-  ''copy\_from\_user''

-  ''vmalloc''

-  ''schedule\_timeout''

-  ''add\_timer''

``<note tip>`` Pentru aceasta folosiți în Vim o comandă de forma '':cs f
g ``<symbol>`` (unde construcția ``<symbol>`` reprezintă numele
simbolului căutat). ``</note>``

Parcurgeți secvența de structuri: 1. ''struct task\_struct'' 2. ''struct
mm\_struct'' 3. ''struct vm\_area\_struct'' 4. ''struct
vm\_operations\_struct'' Adică parcurgeți din aproape în aproape
structurile: accesați o structură și apoi găsiți câmpuri cu tipul de
date al următoarei structuri, accesați-o pe aceasta etc. Rețineți în ce
fișiere sunt definite; o să vă fie utile la alte laboratoare.

``<note tip>`` Pentru a căuta un simbol în Vim (cu suport ''cscope'')
atunci când sunteți plasați cu cursorul pe acesta, folosiți construcția
''Ctrl+]''.

Pentru a reveni în match-ul anterior (înante de căutare/salt) folosiți
construcția ''Ctrl+o''. Pentru a avansa în căutare (pentru a reveni la
match-urile de dinainte de ''Ctrl+o'') folosiți construcția ''Ctrl+i''.
``</note>``

La fel ca mai sus, parcurgeți secvența de apeluri de funcții: 1.
''bio\_alloc'' 2. ''bio\_alloc\_bioset'' 3. ''bvec\_alloc'' 4.
''kmem\_cache\_alloc'' 5. ''slab\_alloc''

``<note tip>`` Citiți secțiunile `cscope <#cscope>`__ sau `LXR
Cross-Reference <#LXR%20Cross-Reference>`__ din laborator. ``</note>``
### Soluții

/\*

-  `Soluții exerciții laborator
   1 <http://elf.cs.pub.ro/so2/res/laboratoare/lab01-sol.zip>`__ \*/
