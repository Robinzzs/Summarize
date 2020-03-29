(switch-to-buffer-other-window "*test*")

(defun hello (name) (insert "Hello " name))
(progn
  (switch-to-buffer-other-window "*test*")
  (hello "you"))

(progn
  (switch-to-buffer-other-window "*test*")
  (erase-buffer)
  (hello "you")
  (other-window 1))

(let ((local-name "you"))
  (switch-to-buffer-other-window "*test*")
  (erase-buffer)
  (hello local-name)
  (other-window 1))

(format "Hello !" )

(defun hello (name)
  (insert (format "hello %s!\n" name)))
(hello "you")

(defun greeting (name)
  (let ((yout-name "Bastien"))
    (insert (format "hello %s\n\nI am %s."
                    name
                    yout-name
                    ))))

(greeting "you")

(read-from-minibuffer "Enter your name: ")

(defun greeting (from-name)
  (let ((your-name (read-from-minibuffer "enter your name:")))
    (insert (format "hello %s\n\nI am %s."
                    from-name
                    your-name
                    ))))
(greeting "bastien")

;; (defun greeting (from-name)
;;   (let ((your-name (read-from-minibuffer "enter your name: ")))
;;     (switch-to-buffer-other-window "*test*")
;;     (erase-buffer)
;;     (insert (format "Hello %s\n\nI am %s" your-name from-name))
;;     (other-window 1)))
(defun greeting (from-name)
  (let ((your-name (read-from-minibuffer "Enter your name: ")))
    (switch-to-buffer-other-window "*test*")
    (erase-buffer)
    (insert (format "Hello %s!\n\nI am %s." your-name from-name))
    (other-window 1)))
(greeting "bastien")



(setq list-of-names '("sarch" "chloe" "mathilde"))

(car list-of-names)

(cdr list-of-names)

(push "stephanis" list-of-names)

(mapcar 'hello list-of-names)hello stephanis!

(defun greeting ()
  (switch-to-buffer-other-window "*test*")
  (erase-buffer)
  (mapcar 'hello list-of-names)
  (other-window 1))
(greeting)

(defun replace-hello-by-bonjour ()
  (switch-to-buffer-other-window "*test*")
  (goto-char (point-min))
  (while (search-forward "hello")
    (replace-match "Bonjour"))
  (other-window 1))

(replace-hello-by-bonjour)

;; (defun hello-to-bonjour ()
;;   (switch-to-buffer-other-window "*test*")
;;   (erase-buffer)
;;   (mapcar 'hello list-of-names)
;;   (goto-char (point-min))
;;   (while (search-forward "hello" nil t)
;;     (replace-match "bonjour"))
;;   (other-window 1))
(defun hello-to-bonjour ()
  (switch-to-buffer-other-window "*test*")
  (erase-buffer)
  ;; 为`list-of-names'中的每个名字调用hello
  (mapcar 'hello list-of-names)
  (goto-char (point-min))
  ;; 将"Hello" 替换为"Bonjour"
  (while (search-forward "hello" nil t)
    (replace-match "Bonjour"))
  (other-window 1))

(hello-to-bonjour)

(defun boldify-names ()
  (switch-to-buffer-other-window "*test*")
  (goto-char (point-min))
  (while (re-search-forward "bonjour \\(.+\\)!" nil t)
    (add-text-properties (match-beginning 1)
                         (match-end 1)
                         (list 'face 'bold)))
  (other-window 1))
(boldify-names)
