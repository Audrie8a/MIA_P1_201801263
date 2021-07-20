   datos = getInicioFin(ListaEspaciosLibres[contador]);
            if (start == datos[0])
            {
                contenido += "\t<td rowspan=" 3 " color=\"orange\">";
                    contenido += string(particion[i].nombre) + " " + Porcentajes(particion[i].size) + "%";
                    contenido += "</td>\n";
            }
            else
            {
                if (n == "p")