<!-- Libit stylesheet
       Vivien Chappelier <vivien.chappelier@irisa.fr>
     adapted from the DocBook-utils Style Sheet for DocBook
     by Eric Bischoff <eric@caldera.de>
-->

<!DOCTYPE style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN" [
  <!ENTITY % html "IGNORE">
  <![%html; [
	<!ENTITY % print "IGNORE">
	<!ENTITY docbook.dsl PUBLIC "-//Norman Walsh//DOCUMENT DocBook HTML Stylesheet//EN" CDATA dsssl>
  ]]>
  <!ENTITY % print "INCLUDE">
  <![%print; [
	<!ENTITY docbook.dsl PUBLIC "-//Norman Walsh//DOCUMENT DocBook Print Stylesheet//EN" CDATA dsssl>
  ]]>
]>

<STYLE-SHEET>

  <STYLE-SPECIFICATION ID="UTILS" USE="DOCBOOK">
    <STYLE-SPECIFICATION-BODY>
;; ===================================================================
;; Generic Parameters
;; (Generic currently means: both print and html)

(define %chapter-autolabel% #t)
(define %section-autolabel% #t)
(define (toc-depth nd) 3)

    </STYLE-SPECIFICATION-BODY>
  </STYLE-SPECIFICATION>

  <STYLE-SPECIFICATION ID="PRINT" USE="UTILS">
    <STYLE-SPECIFICATION-BODY>
;; ===================================================================
;; Print Parameters
;; Call: jade -d docbook-utils.dsl#print

; === Page layout ===
;; (define %paper-type% "A4")		;; use A4 paper - comment this out if needed

; === Media objects ===
;; Allow PDF graphics inclusion 
;; from Yann Dirson <!-- <Yann.Dirson@fr.alcove.com> -->
;; adapted from Ian Castle <!-- <bb@looksystems.co.uk> -->
;;

;; set this to #t using 'jade -V use-eps' to allow building a .ps document
;; instead of a .pdf one
(define use-eps #f)

;; these just add pdf to the 1.71 lists, unless we want to use eps instead
;; this allows to specify both eps and pdf in the mediaobject
(define %graphic-extensions% 
  (if use-eps
      '("eps" "epsf" "gif" "tif" "tiff" "jpg" "jpeg" "png")
      '("pdf" "eps" "epsf" "gif" "tif" "tiff" "jpg" "jpeg" "png")))
(define preferred-mediaobject-notations
  (if use-eps
      '("EPS" "PS" "JPG" "JPEG" "PNG" "linespecific")
      '("PDF" "EPS" "PS" "JPG" "JPEG" "PNG" "linespecific")))
(define preferred-mediaobject-extensions
  (if use-eps
      '("eps" "ps" "jpg" "jpeg" "png")
      '("eps" "ps" "jpg" "jpeg" "png" "pdf")))

; === Rendering ===
(define %head-after-factor% 0.2)	;; not much whitespace after orderedlist head
(define ($paragraph$)			;; more whitespace after paragraph than before
  (make paragraph
    first-line-start-indent: (if (is-first-para)
                                 %para-indent-firstpara%
                                 %para-indent%)
    space-before: (* %para-sep% 4)
    space-after: (/ %para-sep% 4)
    quadding: %default-quadding%
    hyphenate?: %hyphenation%
    language: (dsssl-language-code)
    (process-children)))

(define %default-quadding% 'justify)    ;; justify text

(declare-characteristic heading-level   ;; for PDF bookmarks
  "UNREGISTERED::James Clark//Characteristic::heading-level" 2)

; === Books only ===
(define %generate-book-titlepage% #t)
(define %generate-book-toc% #f)
(define ($generate-chapter-toc$) #f)	;; never generate a chapter TOC in books
;; don't generate lists of tables either
(define ($generate-book-lot-list$) '() )

; === Manpages ===
(define %generate-part-toc% #t)         ;; toc for parts
(define %generate-part-toc-on-titlepage% #f)
(define %generate-part-titlepage% #f)
(define %generate-partintro-on-titlepage% #f)
(define %generate-reference-toc% #t)    ;; toc for references
(define %generate-reference-toc-on-titlepage% #t)

(define %refentry-new-page% #t)         ;; a new page for each manpage

(element refsect1                       ;; ignore refsect1 with role="man"
    (if (equal? (normalize "man") (normalize (attribute-string (normalize "role"))))
	(empty-sosofo)
	($section$)))

    </STYLE-SPECIFICATION-BODY>
  </STYLE-SPECIFICATION>

  <STYLE-SPECIFICATION ID="HTML" USE="UTILS">
    <STYLE-SPECIFICATION-BODY>
;; ===================================================================
;; HTML Parameters
;; Call: jade -d docbook-utils.dsl#html

; === File names ===
(define %root-filename% "index")	;; name for the root html file
(define %html-ext% ".html")		;; default extension for html output files
(define %html-prefix% "")               ;; prefix for all filenames generated (except root)
(define %use-id-as-filename% #t)        ;; if #t uses ID value, if present, as filename
                                        ;;   otherwise a code is used to indicate level
                                        ;;   of chunk, and general element number
                                        ;;   (nth element in the document)
(define use-output-dir #f)              ;; output in separate directory?
(define %output-dir% "HTML")            ;; if output in directory, it's called HTML

; === HTML settings ===
(define %html-pubid% "-//W3C//DTD HTML 4.01 Transitional//EN") ;; Nearly true :-(
(define %html40% #t)

; === Media objects ===
(define preferred-mediaobject-extensions  ;; this magic allows to use different graphical
  (list "png" "jpg" "jpeg"))		;;   formats for printing and putting online
(define acceptable-mediaobject-extensions
  (list "bmp" "gif" "eps" "epsf" "avi" "mpg" "mpeg" "qt"))
(define preferred-mediaobject-notations
  (list "PNG" "JPG" "JPEG"))
(define acceptable-mediaobject-notations
  (list "EPS" "BMP" "GIF" "linespecific"))                                                                                                    
; === Rendering ===
(define %admon-graphics% #t)		;; use symbols for Caution|Important|Note|Tip|Warning


; === Books only ===
(define %generate-book-titlepage% #t)
;;(define %generate-book-toc% #t)
(define ($generate-chapter-toc$) #f)	;; never generate a chapter TOC in books
;; don't generate lists of tables either
(define ($generate-book-lot-list$) '() )

; === Manpages ===
(define %generate-part-toc% #f)         ;; no toc for parts
(define %generate-part-toc-on-titlepage% #f)
(define %generate-part-titlepage% #f)
(define %generate-partintro-on-titlepage% #f)
(define %generate-reference-toc% #f)    ;; no toc for references
(define %generate-reference-toc-on-titlepage% #t)

(define %refentry-new-page% #t)         ;; a new page for each manpage

(element refsect1                       ;; ignore refsect1 with role="man"
    (if (equal? (normalize "man") (normalize (attribute-string (normalize "role"))))
	(empty-sosofo)
	(process-children)))

    </STYLE-SPECIFICATION-BODY>
  </STYLE-SPECIFICATION>

  <EXTERNAL-SPECIFICATION ID="DOCBOOK" DOCUMENT="docbook.dsl">

</STYLE-SHEET>
