% Latex abstract template for CILAMCE 2018 
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{cilamce}

% Package Loading
%
\LoadClass[a4paper,11pt]{article}
\RequirePackage[T1]{fontenc}
\RequirePackage[latin1]{inputenc}
%\RequirePackage[utf8x]{inputenc}
\RequirePackage{amsmath,amsfonts,amssymb,bm}
\RequirePackage{pslatex}
\RequirePackage{graphicx,color}
\RequirePackage[a4paper,body={16cm,25.6cm}]{geometry}

\bibliographystyle{unsrt}

\newcommand{\address}[1]{\def\@address{#1}}

\newcommand{\icacmfont}{\fontfamily{pag}\fontseries{b}\fontshape{sc}\selectfont}



\renewcommand{\thesection}{\arabic{section}.}
\renewcommand{\thesubsection}{\arabic{section}.\arabic{subsection}.}
\renewcommand{\thesubsubsection}{\arabic{section}.\arabic{subsection}.\arabic{subsubsection}}




\renewcommand{\@maketitle}{%
\noindent\begin{flushright} %\large 
\icacmfont CILAMCE 2018 Congress  \\
\small 11-14 November 2018, Paris/Compi{\`e}gne, France \end{flushright}
\par\vspace{1em}
\noindent\textcolor{black}{\rule{\linewidth}{1pt}}
\Large\icacmfont\@title
\par\vspace{1em}\normalfont\normalsize\noindent\@author
\par\vspace{1em}
\noindent\begin{tabular}[t]{@{}l}%
\@address
\end{tabular}
\par\vspace{1em}
\noindent{\rule{\linewidth}{1pt}
\par\vspace{2em}}
}

\renewenvironment{thebibliography}[1]
     {\section*{\large\refname}%
      \small\@mkboth{\MakeUppercase\refname}{\MakeUppercase\refname}%
      \list{\@biblabel{\@arabic\c@enumiv}}%
           {\settowidth\labelwidth{\@biblabel{#1}}%
            \leftmargin\labelwidth
            \advance\leftmargin\labelsep
            \@openbib@code
            \usecounter{enumiv}%
            \let\p@enumiv\@empty
            \renewcommand\theenumiv{\@arabic\c@enumiv}}%
      \sloppy
      \clubpenalty4000
      \@clubpenalty \clubpenalty
      \widowpenalty4000%
      \sfcode`\.\@m}
     {\def\@noitemerr
       {\@latex@warning{Empty `thebibliography' environment}}%
      \endlist}
\setlength{\parskip}{0ex}


