INSTALL=sudo apt-get install
TEX_LIBS=texlive-latex-base texlive-fonts-recommended texlive-science texlive-fonts-extra texlive-latex-extra texlive-publishers texlive-humanities


all:
	@$(INSTALL) $(TEX_LIBS)
	@( pdflatex bare_jrnl > /dev/null)
	@( bibtex bare_jrnl)
	@( pdflatex bare_jrnl > /dev/null)
	@( pdflatex bare_jrnl > /dev/null)
