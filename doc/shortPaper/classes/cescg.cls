%%
%% This is file `cescg.cls',
%% generated with the docstrip utility.
%%
%% The original source files were:
%%
%% cescg.dtx  (with options: `class')
%% 
%% IMPORTANT NOTICE:
%% 
%% For the copyright see the source file.
%% 
%% Any modified versions of this file must be renamed
%% with new filenames distinct from cescg.cls.
%% 
%% For distribution of the original source see the terms
%% for copying and modification in the file cescg.dtx.
%% 
%% This generated file may be distributed as long as the
%% original source files, as listed above, are part of the
%% same distribution. (The sources need not necessarily be
%% in the same archive or directory.)
%% Copyright (c) 1999, 2005
%%               Institute of Computer Graphics and Algorithms
%%               TU Vienna, Austria
%% Based on the LaTeX2e document class SCCG
%% Copyright (C) 1999 Pavel Chalmoviansky
%%                    Katedra geometrie
%%                    Faculty of Mathematics and Physics
%%                    Comenius University, Bratislava
%%                    chalmo@fmph.uniba.sk
%%
%% This file is distributed in the hope that it will be useful,
%% but WITHOUT ANY WARRANTY; without even the implied warranty of
%% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
%%
%% \CharacterTable
%%  {Upper-case    \A\B\C\D\E\F\G\H\I\J\K\L\M\N\O\P\Q\R\S\T\U\V\W\X\Y\Z
%%   Lower-case    \a\b\c\d\e\f\g\h\i\j\k\l\m\n\o\p\q\r\s\t\u\v\w\x\y\z
%%   Digits        \0\1\2\3\4\5\6\7\8\9
%%   Exclamation   \!     Double quote  \"     Hash (number) \#
%%   Dollar        \$     Percent       \%     Ampersand     \&
%%   Acute accent  \'     Left paren    \(     Right paren   \)
%%   Asterisk      \*     Plus          \+     Comma         \,
%%   Minus         \-     Point         \.     Solidus       \/
%%   Colon         \:     Semicolon     \;     Less than     \<
%%   Equals        \=     Greater than  \>     Question mark \?
%%   Commercial at \@     Left bracket  \[     Backslash     \\
%%   Right bracket \]     Circumflex    \^     Underscore    \_
%%   Grave accent  \`     Left brace    \{     Vertical bar  \|
%%   Right brace   \}     Tilde         \~}
%%
\NeedsTeXFormat{LaTeX2e}[1994/06/01]
\ProvidesClass{cescg}
            [2005/11/29 v0.1.4
 CESCG proceedings document class]
\newif\ifcescg@camera
\newif\ifcescg@preprint
\newif\ifcescg@review
\newcommand{\cescg@columnmode}{}
\newcommand{\cescg@pointsize}{}
\newif\ifcescg@doublespaced
\newif\ifcescg@flushtitleleft
\DeclareOption{cameraready}{%
    \cescg@cameratrue%
    \cescg@preprintfalse%
    \cescg@reviewfalse%
    \renewcommand{\cescg@columnmode}{twocolumn}%
    \cescg@doublespacedfalse%
    \renewcommand{\cescg@pointsize}{10pt}}
\DeclareOption{review}{%
    \cescg@camerafalse%
    \cescg@preprintfalse%
    \cescg@reviewtrue%
    \renewcommand{\cescg@columnmode}{twocolumn}%
    \cescg@doublespacedfalse%
    \renewcommand{\cescg@pointsize}{10pt}}
\DeclareOption{widereview}{%
    \cescg@camerafalse%
    \cescg@preprintfalse%
    \cescg@reviewtrue%
    \renewcommand{\cescg@columnmode}{onecolumn}%
    \cescg@doublespacedtrue%
    \renewcommand{\cescg@pointsize}{12pt}}
\DeclareOption{preprint}{%
    \cescg@camerafalse%
    \cescg@preprinttrue%
    \cescg@reviewfalse%
    \renewcommand{\cescg@columnmode}{twocolumn}%
    \cescg@doublespacedfalse%
    \renewcommand{\cescg@pointsize}{10pt}}
\ExecuteOptions{cameraready}
\newcommand{\cescg@optwarning}[1]{%
    \ifcescg@camera
        \ClassWarningNoLine{cescg}%
            {#1 option used in camera-ready mode.\MessageBreak
             This violates submission specifications}
    \fi
}
\DeclareOption{onecolumn}{%
    \cescg@optwarning{onecolumn}
    \renewcommand{\cescg@columnmode}{onecolumn}}
\DeclareOption{twocolumn}{%
    \renewcommand{\cescg@columnmode}{twocolumn}}
\DeclareOption{10pt}{%
    \cescg@optwarning{10pt}
    \renewcommand{\cescg@pointsize}{10pt}}
\DeclareOption{11pt}{%
    \cescg@optwarning{11pt}
    \renewcommand{\cescg@pointsize}{11pt}}
\DeclareOption{12pt}{%
    \cescg@optwarning{12pt}
    \renewcommand{\cescg@pointsize}{12pt}}
\DeclareOption{singlespace}{%
    \cescg@doublespacedfalse}
\DeclareOption{doublespace}{%
    \cescg@optwarning{doublespace}
    \cescg@doublespacedtrue}
\DeclareOption{flushtitleleft}{%
    \cescg@flushtitlelefttrue}
\DeclareOption{flushtitleright}{%
    \cescg@flushtitleleftfalse}
\ExecuteOptions{flushtitleright}
\DeclareOption{titlepage}{%
    \OptionNotUsed%
    \ClassWarningNoLine{cescg}{titlepage option not allowed}}
\DeclareOption{landscape}{%
    \OptionNotUsed%
    \ClassWarningNoLine{cescg}{landscape option not allowed}}
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{article}}
\ProcessOptions
\PassOptionsToClass{a4paper}{article}
\PassOptionsToClass{\cescg@columnmode}{article}
\ifdim\cescg@pointsize>10pt
    \PassOptionsToClass{\cescg@pointsize}{article}
\fi
\LoadClass{article}
\RequirePackage{mathptmx}
\RequirePackage[scaled=.90]{helvet}
\RequirePackage{courier}
\newcommand{\cescg@preprinttext}{To appear in the CESCG conference proceedings}
\newcommand{\preprinttext}[1]{\renewcommand{\cescg@preprinttext}{#1}}
\newcommand{\cescg@footertext}{Proceedings of CESCG 2013: The 17th Central European Seminar on Computer Graphics (non-peer-reviewed)}
\newcommand{\footertext}[1]{\renewcommand{\cescg@footertext}{#1}}
\newcommand{\ifcamera}[1]{\ifcescg@review \else #1 \fi}
\newcommand{\ifreview}[1]{\ifcescg@review #1 \fi}
\newcommand{\ifcameraelse}[2]{\ifcescg@review #2 \else #1 \fi}
\newcommand{\ifreviewelse}[2]{\ifcescg@review #1 \else #2 \fi}
\setlength{\textheight}{24cm}
\setlength{\topmargin}{-0.85cm}
\setlength{\headheight}{0.5cm}
\setlength{\headsep}{0.65cm}
\setlength{\footskip}{1.25cm}
\flushbottom
\setlength{\hoffset}{-0.54cm}
\setlength{\textwidth}{17cm}
\setlength{\oddsidemargin}{0cm}
\setlength{\evensidemargin}{0cm}
\setlength{\marginparsep}{0cm}
\setlength{\marginparwidth}{0pt}
\setlength{\columnsep}{0.7cm}
\setlength{\parindent}{1em}
\renewcommand{\ps@plain}%
    {%
    \ifcescg@preprint%
\renewcommand{\@oddhead}{\hfil\textit{\large\cescg@preprinttext}\hfil}%
    \else%
\renewcommand{\@oddhead}{}%
    \fi%
    \ifcescg@camera%
    \renewcommand{\@oddfoot}{\hfil\textit{\cescg@footertext}\hfil}%
    \else%
\renewcommand{\@oddfoot}{\hfil\textrm{\thepage}\hfil}%
    \fi%
    \renewcommand{\@evenhead}{\@oddhead}%
    \renewcommand{\@evenfoot}{\@oddfoot}%
    }
\renewcommand{\ps@empty}%
    {%
    \ifcescg@preprint%
\renewcommand{\@oddhead}{\hfil\textit{\large\cescg@preprinttext}\hfil}%
    \else%
    \renewcommand{\@oddhead}{}%
    \fi%
    \renewcommand{\@oddfoot}{\hfil\textit{\cescg@footertext}\hfil}%
    \renewcommand{\@evenhead}{\@oddhead}%
    \renewcommand{\@evenfoot}{\@oddfoot}%
    }
\pagestyle{plain}
\newcommand{\cescg@sectionfont}{}
\newcommand{\sectionfont}[1]{\renewcommand{\cescg@sectionfont}{#1}}
\renewcommand\section{\@startsection {section}{1}{\z@}%
                {-3.5ex \@plus -1ex \@minus -.2ex}%
                {2.3ex \@plus.2ex}%
                {\reset@font\Large\sffamily\bfseries\cescg@sectionfont}}
\renewcommand\subsection{\@startsection{subsection}{2}{\z@}%
                {-3.25ex\@plus -1ex \@minus -.2ex}%
                {1.5ex \@plus .2ex}%
                {\reset@font\large\sffamily\bfseries\cescg@sectionfont}}
\renewcommand\subsubsection{\@startsection{subsubsection}{3}{\z@}%
                {-3.25ex\@plus -1ex \@minus -.2ex}%
                {1.5ex \@plus .2ex}%
                {\reset@font\normalsize\sffamily\bfseries\cescg@sectionfont}}
\renewcommand\paragraph{\@startsection{paragraph}{4}{\z@}%
                {3.25ex \@plus1ex \@minus.2ex}%
                {-1em}%
                {\reset@font\normalsize\sffamily\bfseries\cescg@sectionfont}}
\renewcommand\subparagraph{\@startsection{subparagraph}{5}{\parindent}%
                {3.25ex \@plus1ex \@minus .2ex}%
                {-1em}%
                {\reset@font\normalsize\sffamily\bfseries\cescg@sectionfont}}
\newcommand{\cescg@empty}{}
\newcommand{\cescg@affiliation}{}
\newcommand{\affiliation}[1]{\renewcommand{\cescg@affiliation}{#1}}
\newcommand{\cescg@supervisor}{}
\newcommand{\supervisor}[1]{\renewcommand{\cescg@supervisor}{\emph{Supervised by: {#1}}}}
\newcommand{\cescg@category}{}
\newcommand{\category}[1]{\renewcommand{\cescg@category}{#1}}
\newcommand{\cescgcategory}[1]{\category{#1}}
\newcommand{\cescg@format}{}
\newcommand{\format}[1]{\renewcommand{\cescg@format}{#1}}
\newcommand{\cescgformat}[1]{\format{#1}}
\newlength{\titlespace}
\setlength{\titlespace}{0.25in}
\ifcescg@review
    \renewcommand{\@maketitle}{%
        \begin{flushright}%
            \renewcommand{\thanks}[1]{}
            {\LARGE\sffamily\bfseries\cescg@sectionfont \@title \par}%
            \vspace{1\baselineskip}%
            {Category: \cescg@category \par}%
            \vspace{\titlespace}%
        \end{flushright} \par%
    }
\else
    \renewcommand{\@maketitle}{%
        \begin{center}%
            {\LARGE\sffamily\bfseries\cescg@sectionfont \@title \par}%
            \vspace{1\baselineskip}%
            \large
            \begin{tabular}[t]{@{}c@{}}%
              \@author
            \end{tabular}\par%
\cescg@supervisor\par%
            \ifx\cescg@affiliation\cescg@empty \else%
                \vspace{1\baselineskip}%
                \cescg@affiliation\par%
            \fi%
            \vspace{\titlespace}%
        \end{center}%
        \par%
    }
\fi
\let\cescg@origmaketitle\maketitle%
\let\and\crcr%
\let\cescg@origand\and%
\renewcommand{\maketitle}{%
    \let\cescg@title\@title%
    \let\cescg@author\@author%
    \cescg@origmaketitle%
    \thispagestyle{empty}%
    \ifcescg@doublespaced%
        \renewcommand{\baselinestretch}{1.66}\selectfont%
    \fi
}
\newtoks\cescg@abs
\ifcescg@review
    \long\def\cescg@add#1{\global\cescg@abs\expandafter{\the\cescg@abs#1}}
    \long\def\cescg@collect{%
        \global\cescg@abs{}%
        \let\abstract\cescg@@collect
        \abstract
    }
    \long\def\cescg@@collect#1\end#2{%
        \def\@tempa{#2}%
        \ifx\@tempa\@currenvir
            \cescg@add{#1}%
            \edef\abstract{\noexpand\end{\@tempa}}%
        \else
            \cescg@add{#1\end{#2}}%
        \fi
        \abstract
    }
    \newcommand{\cescg@modify}{%
        \let\cescg@origabs\abstract%
        \let\cescg@origendabs\endabstract%
        \renewenvironment{abstract}%
            {\cescg@collect}%
            {\begingroup
                \let\abstract\cescg@origabs
                \let\endabstract\cescg@origendabs
                \begin{abstract} \the\cescg@abs \end{abstract}
            \endgroup}%
    }
    \AtBeginDocument{\cescg@modify}
\fi
\newcommand{\keywordlist}{%
    \vspace{0.5\baselineskip}%
    \par\noindent \textbf{Keywords: } \cescg@keywords%
}
\newcommand{\cescg@contactname}{}
\newcommand{\contactname}[1]{\renewcommand{\cescg@contactname}{#1}}
\newcommand{\cescg@contactaddress}{}
\newcommand{\contactaddress}[1]{\renewcommand{\cescg@contactaddress}{#1}}
\newcommand{\cescg@contactphone}{}
\newcommand{\contactphone}[1]{\renewcommand{\cescg@contactphone}{#1}}
\newcommand{\cescg@contactfax}{}
\newcommand{\contactfax}[1]{\renewcommand{\cescg@contactfax}{#1}}
\newcommand{\cescg@contactemail}{}
\newcommand{\contactemail}[1]{\renewcommand{\cescg@contactemail}{#1}}
\newcommand{\cescg@keywords}{}
\newcommand{\keywords}[1]{\renewcommand{\cescg@keywords}{#1}}
\newcommand{\cescg@estpages}{}
\newcommand{\estpages}[1]{\renewcommand{\cescg@estpages}{#1}}
\newif\ifcescg@cover
\ifcescg@review
    \cescg@covertrue
\else
    \cescg@coverfalse
\fi
\newcommand{\suppresscover}{\cescg@coverfalse}
\newcommand{\cescg@coverpage}{%
    \begin{titlepage}%
        \def\thanks##1{}
        \let\and\cescg@origand
        \normalsize
        \renewcommand{\baselinestretch}{1}\selectfont
        \begin{center}%
            \vspace*{\fill}
            {\LARGE\sffamily\bfseries\cescg@sectionfont \cescg@title \par}%
            \vspace{2\baselineskip}%
            {\large
                \begin{tabular}[t]{c}%
                \cescg@author
             \end{tabular}\par%
            }%
            \vspace{1\baselineskip}%
{\large \cescg@supervisor \par}%
\vspace{1\baselineskip}%
            {\large \cescg@affiliation \par}%
            \addvspace{3\baselineskip}%
            {Category: \cescg@category \par}%
            \vspace{0.5\baselineskip}%
            {Format: \cescg@format \par}%
            \vspace{3\baselineskip}%
            \begin{tabular}{ll}
                Contact: & \cescg@contactname \\[1\baselineskip]
                         & \begin{tabular}[b]{@{}l@{}}
                               \cescg@contactaddress
                               \end{tabular} \\[1\baselineskip]
                phone: & \cescg@contactphone \\
                fax: & \cescg@contactfax \\
                email: & \cescg@contactemail
            \end{tabular}\par%
            \vspace*{3\baselineskip}%
            {Estimated \# of pages: \cescg@estpages \par}%
            \vspace*{\baselineskip}%
            {Keywords: \cescg@keywords \par}%
            \vspace*{\fill}%
            \begin{minipage}{5in}%
                \the\cescg@abs
            \end{minipage}\par%
            \vspace*{\fill}
        \end{center}%
    \end{titlepage}%
}
\AtEndDocument{\ifcescg@cover \cescg@coverpage \fi}
\InputIfFileExists{cescg.cfg}
        {\typeout{***************************************^^J%
                  * Local config file cescg.cfg used    *^^J%
                  ***************************************}}
        {}

\endinput
%%
%% End of file `cescg.cls'.
