# maquina de mate
Primero armamos el circuito y el código necesario para corroborar que todo funciona correctamente. Luego, probamos como funcionaría distintos detalles de implementación, por ejemplo, las dimensiones de la lista que guarda los horarios en los cuáles hay que hacer el mate y el funcionamiento de la interfaz.

La interfaz tiene 

Para la lista de horarios, pensamos en hacer un array tridimensional, donde separaríamos los días, y dentro de ellos, dos arrays, uno para las horas y otro para los minutos. Estos mismos estarían relacionados mediante el índice, conformando el primer elemento de ambas listas un solo horario, por ejemplo. Abandonamos esta idea porque no era compatible con la navegación de horarios prevista usando un potenciómetro, y sería complicado asignarle una posición bidimensional a las medidas de este componente. 

Decidimos hacer un array de strings para este propósito, donde cada string tiene 5 números, correspondiendo el primero al día de la semana, del 1 al 7, y los demás a la hora y minuto, en un formato de 24 horas sin separador.