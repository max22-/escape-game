(display "hello")

(sensor-max 0 2000)

(= d 2000)

(do
    (= i 0)
    (while (< i 7)
	   (display "Capteur ")
	   (display i)
	   (display "min")
	   (display (sensor-min i d))
	   (display "")
	   (= i (+ i 1))))



(restart)
