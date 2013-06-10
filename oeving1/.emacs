;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;
;;;; Emacs oppstartfil
;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; syntax-highlighting
(global-font-lock-mode t)
;; ikke bruk tab i innrykk
(setq-default indent-tabs-mode nil)
;; tab-størrelse
(setq default-tab-width 8)
;; vis linjenummer
(line-number-mode t)
;; vis kolonnenummer
(column-number-mode t)
;; støtte for rullehjul
(mouse-wheel-mode t)
;; bruk visible bell
(setq visible-bell t)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; innstillinger for alle cc-moduser

(add-hook 'c-mode-common-hook 
          (lambda () 
            ;; innrykk på 2 tegn
            (setq c-basic-offset 2)
            ;; innrykk på case-labeler også
            (c-set-offset 'case-label '+)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; innstillinger for VHDL-modus

(add-hook 'vhdl-mode-hook
          (lambda () 
            ;; vhdl-mode tab gir alltid innrykk
            (local-set-key [tab] 'vhdl-indent-line)
            ;; skru av electric mode
            (setq vhdl-electric-mode nil)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; tegnsett

(set-language-environment "UTF-8")
(setq locale-coding-system 'utf-8)
(set-terminal-coding-system 'utf-8)
(set-keyboard-coding-system 'utf-8)
(set-selection-coding-system 'utf-8)
(prefer-coding-system 'utf-8)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
