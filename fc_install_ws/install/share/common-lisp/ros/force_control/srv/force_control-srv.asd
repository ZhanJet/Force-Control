
(cl:in-package :asdf)

(defsystem "force_control-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "SetFloat64" :depends-on ("_package_SetFloat64"))
    (:file "_package_SetFloat64" :depends-on ("_package"))
    (:file "SetString" :depends-on ("_package_SetString"))
    (:file "_package_SetString" :depends-on ("_package"))
  ))