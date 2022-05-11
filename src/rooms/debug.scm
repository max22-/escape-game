(display "hello")

(= d 10000)

(sensor-max 0 d)

(do
    (= i 0)
    (while (< i 7)
	   (display "Capteur ")
	   (display i)
	   (display "max")
	   (display (sensor-max i d))
	   (display "")
	   (= i (+ i 1))))


(do
    (= i 0)
    (while (< i 7)
	   (display "Capteur ")
	   (display i)
     	   (display (sensor i))
	   (display "")
	   (= i (+ i 1))))

(restart)

(do
    (= ts (millis))
    (while (< (millis) (+ ts 10000))
	   (display (sensor 0))))
	      
