;; Auto-generated. Do not edit!


(when (boundp 'force_control::SetFloat64)
  (if (not (find-package "FORCE_CONTROL"))
    (make-package "FORCE_CONTROL"))
  (shadow 'SetFloat64 (find-package "FORCE_CONTROL")))
(unless (find-package "FORCE_CONTROL::SETFLOAT64")
  (make-package "FORCE_CONTROL::SETFLOAT64"))
(unless (find-package "FORCE_CONTROL::SETFLOAT64REQUEST")
  (make-package "FORCE_CONTROL::SETFLOAT64REQUEST"))
(unless (find-package "FORCE_CONTROL::SETFLOAT64RESPONSE")
  (make-package "FORCE_CONTROL::SETFLOAT64RESPONSE"))

(in-package "ROS")





(defclass force_control::SetFloat64Request
  :super ros::object
  :slots (_data ))

(defmethod force_control::SetFloat64Request
  (:init
   (&key
    ((:data __data) 0.0)
    )
   (send-super :init)
   (setq _data (float __data))
   self)
  (:data
   (&optional __data)
   (if __data (setq _data __data)) _data)
  (:serialization-length
   ()
   (+
    ;; float64 _data
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float64 _data
       (sys::poke _data (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float64 _data
     (setq _data (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(defclass force_control::SetFloat64Response
  :super ros::object
  :slots (_success _message ))

(defmethod force_control::SetFloat64Response
  (:init
   (&key
    ((:success __success) nil)
    ((:message __message) "")
    )
   (send-super :init)
   (setq _success __success)
   (setq _message (string __message))
   self)
  (:success
   (&optional __success)
   (if __success (setq _success __success)) _success)
  (:message
   (&optional __message)
   (if __message (setq _message __message)) _message)
  (:serialization-length
   ()
   (+
    ;; bool _success
    1
    ;; string _message
    4 (length _message)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _success
       (if _success (write-byte -1 s) (write-byte 0 s))
     ;; string _message
       (write-long (length _message) s) (princ _message s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _success
     (setq _success (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; string _message
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _message (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass force_control::SetFloat64
  :super ros::object
  :slots ())

(setf (get force_control::SetFloat64 :md5sum-) "3f47c51e9da05852f0d7f484f9279955")
(setf (get force_control::SetFloat64 :datatype-) "force_control/SetFloat64")
(setf (get force_control::SetFloat64 :request) force_control::SetFloat64Request)
(setf (get force_control::SetFloat64 :response) force_control::SetFloat64Response)

(defmethod force_control::SetFloat64Request
  (:response () (instance force_control::SetFloat64Response :init)))

(setf (get force_control::SetFloat64Request :md5sum-) "3f47c51e9da05852f0d7f484f9279955")
(setf (get force_control::SetFloat64Request :datatype-) "force_control/SetFloat64Request")
(setf (get force_control::SetFloat64Request :definition-)
      "float64 data
---
bool success
string message

")

(setf (get force_control::SetFloat64Response :md5sum-) "3f47c51e9da05852f0d7f484f9279955")
(setf (get force_control::SetFloat64Response :datatype-) "force_control/SetFloat64Response")
(setf (get force_control::SetFloat64Response :definition-)
      "float64 data
---
bool success
string message

")



(provide :force_control/SetFloat64 "3f47c51e9da05852f0d7f484f9279955")


