if (cont_arg_float > 0)
            {
                if ((espaco = args_espaco(aux, i)) == 0)
                {
                    for (j = i+1; aux->linha[j] != '\0'; j++)
                    {
                        if (aux->linha[j] != ' ')
                        {
                            if (isdigit(aux->linha[j+1]))
                            {
                                jump_pula = pega_int(aux, j);
                                continue;
                            }
                            else
                            {
                                if (!isdigit(aux->linha[j]))
                                {
                                    if(aux->linha[j] == '\n')
                                    {
                                        i = j;
                                        break;
                                    }
                                    else
                                    {
                                        printf("Um erro ocorreu, olhe o painel de log para mais informacoes.\n");
                                        fprintf(erro_log, "Erro coluna %d: A funcao '%s' so aceita numeros inteiros em seus parametros.", aux->coluna, funcao);
                                        return -1;
                                    }
                                }
                                cont_parametro++;
                            }
                        }
                    }
                }
            }