
<HTML>
<HEAD>
<TITLE>Homework #2 - CSCI 531, Spring 2015</TITLE>
<LINK HREF="../../cs531bc.css" REL="stylesheet" TYPE="text/css">
<LINK REV="made" HREF="mailto:bill.cheng@acm.org"></HEAD>
<BODY BGCOLOR="#FFFFFF" LINK="#D02090" VLINK="#996600" ALINK="#990000">

<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0 WIDTH=100%>
<TR>
    
    <TD ALIGN=LEFT VALIGN=TOP>
    <!-- COL 3, every page should be different -->
        <TABLE BORDER=0>
        <TR><TD COLSPAN=3 ALIGN=LEFT>
The purpose of this assignment is to get you familiar with visual
cryptography.  Please see
<A HREF="http://www.cacr.math.uwaterloo.ca/~dstinson/visual.html">
Doug Stinson's visual cryptography page</A> for an introduction.
Our assignment will be slighly different from what's on Doug Stinson's page
</TD>
</TR>
<TR><TD>&nbsp;</TD></TR>
<!-- Section -->
<TR><TD COLSPAN=3 ALIGN=LEFT BGCOLOR="#000000" WIDTH=100%>
<FONT COLOR="#ffffff"><A
NAME="compile"><B>Compiling</B></A></FONT>
</TD>
</TR>
<!-- Top Section -->
<TR><TD COLSPAN=3 ALIGN=LEFT>
<PRE>
make hw2</PRE>
an executable named <B>hw2</B> is created.

            </TD>
        </TR>
        <TR><TD>&nbsp;</TD></TR>
        <!-- Section -->
        <TR><TD COLSPAN=3 ALIGN=LEFT BGCOLOR="#000000" WIDTH=100%>
                <FONT COLOR="#ffffff"><A
                NAME="syntax"><B>Commandline Syntax & Program
                Output</B></A></FONT>
            </TD>
        </TR>
        <!-- Top Section -->
        <TR><TD COLSPAN=3 ALIGN=LEFT>
The commandline syntax for <B>hw2</B> is as follows:

<PRE>
    <B>hw2</B> <B>stream</B> <B>-p=</B>pphrase <B>-l=</B>len
    <B>hw2</B> <B>encrypt</B> <B>-p=</B>pphrase <B>-out=</B>name [pbmfile]
    <B>hw2</B> <B>merge</B> pbmfile1 pbmfile2
    <B>hw2</B> <B>decrypt</B> [pbmfile]</PRE>

<P>

Square bracketed items are optional.
Follows the UNIX convention that <B>commandline options</B>
can come in any order.  (Note: a <B>commandline option</B> is a
commandline argument that begins with a <B>-</B> character in a
commandline syntax specification.)
If <TT>pbmfile</TT> is not specified, the program
reads from <TT>stdin</TT>.
<TT>pbmfile</TT> refers to a file in the
<A HREF="http://netpbm.sourceforge.net/doc/pbm.html">
<B>Portable Bitmap (or PBM)</B></A> format with <B>P4</B>
being the magic number.
Output of the program goes to <TT>stdout</TT> and
error messages go to <TT>stderr</TT>.

<p>
The meaning of the commands are:

<BLOCKQUOTE>
<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
    <TR><TD ALIGN=LEFT VALIGN=TOP><B>stream</B></TD>
        <TD VALIGN=TOP>&nbsp;&nbsp;:&nbsp;&nbsp;</TD>
        <TD VALIGN=TOP>A <A HREF="#stream">simple stream cipher</A>.<BR>
                       The ASCII string <TT>pphrase</TT>
                       contains the passphrase (which must not be empty) for
                       <A HREF="#stream">key generation</A>
                       and <TT>len</TT> is the number of bytes to output.
        </TD>
    </TR>
    <TR><TD>&nbsp;</TD></TR>
    <TR><TD ALIGN=LEFT VALIGN=TOP><B>encrypt</B></TD>
        <TD VALIGN=TOP>&nbsp;&nbsp;:&nbsp;&nbsp;</TD>
        <TD VALIGN=TOP>Encrypts an input PBM file with the <A HREF="#stream">
                       simple stream cipher</A>.  This should create 2 output
                       files having the same filename as <TT>name</TT> but
                       one with <TT>.1.pbm</TT> appended and the other
                       with <TT>.2.pbm</TT> appended to the filenames.
                       For example, if
                       <TT>name</TT> is <TT>foo</TT>, the output
                       file names must be <TT>foo.1.pbm</TT> and
                       <TT>foo.2.pbm</TT>.
                       The relationship between an input pixel, a key bit,
                       and pixels in the output files is specified in the
                       <A HREF="#table">table above</A>.
        </TD>
    </TR>
    <TR><TD>&nbsp;</TD></TR>
    <TR><TD ALIGN=LEFT VALIGN=TOP><B>merge</B></TD>
        <TD VALIGN=TOP>&nbsp;&nbsp;:&nbsp;&nbsp;</TD>
        <TD VALIGN=TOP>Create an output PBM file by merging the bits
                       of PBM files <TT>pbmfile1</TT> and <TT>pbmfile2</TT>.
                       A bit value of 1 is treated as
                       a black pixel and a bit value
                       of 0 is treated as a white pixel.
                       When merging a bit of 1
                       and a bit of 0, the output must be a 1.
                       You must use 0 as the padding bit at the end of a row
                       in the output PBM file.
        </TD>
    </TR>
    <TR><TD>&nbsp;</TD></TR>
    <TR><TD ALIGN=LEFT VALIGN=TOP><B>decrypt</B></TD>
        <TD VALIGN=TOP>&nbsp;&nbsp;:&nbsp;&nbsp;</TD>
        <TD VALIGN=TOP>Treats the input PBM file as the result of the merge
                       command and perform the reverse of the encryption
                       algorithm.  The resulting image size should be
                       1/4 of the original input.
        </TD>
    </TR>
</TABLE>
</BLOCKQUOTE>

<P>

The output for various commands are as follows.

<BLOCKQUOTE>
<TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>
    <TR><TD ALIGN=LEFT VALIGN=TOP><B>stream</B></TD>
        <TD VALIGN=TOP>&nbsp;&nbsp;:&nbsp;&nbsp;</TD>
        <TD VALIGN=TOP>
            The output is a <I>binary</I> stream of bytes of length
            <TT>len</TT>.
        </TD>
    </TR>
    <TR><TD>&nbsp;</TD></TR>
    <TR><TD ALIGN=LEFT VALIGN=TOP><B>encrypt</B></TD>
        <TD VALIGN=TOP>&nbsp;&nbsp;:&nbsp;&nbsp;</TD>
        <TD VALIGN=TOP>
            There is no output for this command.  The side effect
            of this command is to create two files in the P4 PBM format.
        </TD>
    </TR>
    <TR><TD>&nbsp;</TD></TR>
    <TR><TD ALIGN=LEFT VALIGN=TOP><B>merge</B></TD>
        <TD VALIGN=TOP>&nbsp;&nbsp;:&nbsp;&nbsp;</TD>
        <TD VALIGN=TOP>
            The output of this command is a file in the P4 PBM format.
        </TD>
    </TR>
    <TR><TD>&nbsp;</TD></TR>
    <TR><TD ALIGN=LEFT VALIGN=TOP><B>decrypt</B></TD>
        <TD VALIGN=TOP>&nbsp;&nbsp;:&nbsp;&nbsp;</TD>
        <TD VALIGN=TOP>
            The output of this command is a file in the P4 PBM format.
        </TD>
    </TR>
    <TR><TD>&nbsp;</TD></TR>
</TABLE>
</BLOCKQUOTE>

<P>

            </TD>
        </TR>
        <TR><TD>&nbsp;</TD></TR>
        <!-- Section -->
        <TR><TD COLSPAN=3 ALIGN=LEFT BGCOLOR="#000000" WIDTH=100%>
                <FONT COLOR="#ffffff"><A NAME="stream">
                <B>Key Generation for a Simple Stream Cipher</B></A></FONT>
            </TD>
        </TR>
        <!-- Top Section -->
        <TR><TD COLSPAN=3 ALIGN=LEFT>
Let <TT>p</TT> be a null-terminated string containing the passphrase.
Please ues the following algorithm (or equivalent) to generate
key stream for our simple stream cipher.  (Please make appropriate
modifications to make it work according to spec.)
At the end of every iteration, 8 bytes of the key stream are generated.
    <PRE>
    #include &lt;sys/types.h&gt;
    #include &lt;openssl/md5.h&gt;  /* please read <A HREF="../../openssl.html">this</A> */

    char md5_buf[MD5_DIGEST_LENGTH];
    int len=strlen(p)+2+MD5_DIGEST_LENGTH;
    char *s=malloc(len+1);
    int i=0;

    MD5(p, strlen(p), md5_buf);
    for (;;) {
        sprintf(&s[MD5_DIGEST_LENGTH], "%02d%s", i, p);
        memcpy(s, md5_buf, MD5_DIGEST_LENGTH);
        MD5(s, len, md5_buf);
        fwrite(md5_buf, 1, sizeof(md5_buf)>>1, stdout);
        if (++i == 100) i = 0;
    }
    free(s);</PRE>
The idea here is that the buffer whose MD5 checksum is getting
computed has 3 parts.  The first part is 16 bytes long and it
is the MD5 checksum of the previous value of the buffer.  The
2nd part is an ASCII string of length 2 that takes on the value
from <TT>"00"</TT> through <TT>"99"</TT>.
The 3rd part is the passphrase.  Initially, the first
part is the MD5 checksum of the passphrase.

<P>

For example, if <TT>pphrase</TT> is <TT>"yesnomaybe"</TT>,
the 3 parts of the buffer is, initially, in hexstring representation,
<TT>69cbe89bfa6370e0ab07df9a6096d3d2</TT>, 
<TT>3030</TT>, and
<TT>7965736e6f6d61796265</TT>.
The MD5 checksum of this buffer is
<TT>cf686d0977103817538bfefdf271be2f</TT>.
Therefore, the first 8 bytes of our simple stream cipher output
is <TT>cf686d0977103817</TT>.

<P>

In the 2nd interation,
The 3 parts of the buffer is, hexstring representation,
<TT>cf686d0977103817538bfefdf271be2f</TT>, 
<TT>3031</TT>, and
<TT>7965736e6f6d61796265</TT>.
The MD5 checksum of this buffer is
<TT>aa632cf32cd1740c1544816ac51fa9d0</TT>.
Therefore, the first 8 bytes of our simple stream cipher output
is <TT>aa632cf32cd1740c</TT>.

<P>

Information about the MD5 algorithm can be found at
<A HREF="http://www.openssl.org/docs/crypto/md5.html">
http://www.openssl.org/docs/crypto/md5.html</A>.

<P>

To use <TT>openssl</TT> on <TT>nunki.usc.edu</TT>, please
see the <A HREF="../../openssl.html">additional notes on <TT>openssl</TT></A>.

<P>

<I>(Of course, by now you should know that it is not a good idea to generate
keys this way for RC4.  We are doing it this way to make this HW reasonably
easy to grade.)</I>
            </TD>
        </TR>
        <TR><TD>&nbsp;</TD></TR>
      
</BODY>
</HTML>

