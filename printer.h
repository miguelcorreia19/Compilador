void printAddress(Address p)
{
	if(p == NULL)
		return;
	switch (p->kind) 
    {
    	case C_INT:
    		fprintf(f, "%d", p->content.val);
    	break;

    	case C_VAR:
    		fprintf(f, "%s", p->content.var);
    	break;

    	default:
		    fprintf(f, "*undefined_1*");
    }
}

void printInstr(Instr inst)
{
	int label = 0;
	switch (inst->op) 
    {
	    case C_ADD: 
	    	fprintf(f, "add\t");  
	     	break;
	 
	    case C_SUB: 
	      	fprintf(f, "sub\t");
	      	break;

	    case C_DIV: 
	      	fprintf(f, "div\t");
	      	break;       
	 
	    case C_MUL: 
	      	fprintf(f, "mul\t");
	      	break;
	 
	 	case C_LW:
	 		fprintf(f, "lw\t");
	 		break;

	 	case C_LI:
	 		fprintf(f, "li\t");
	 		break;

	 	case C_SW:
	 		fprintf(f, "sw\t");
	 		break;
	    
	    case C_MOVE:
	 		fprintf(f, "move\t");
	 		break;
	    
	    case C_BEQ:
	 		fprintf(f, "beq\t");
        	break;

        case C_BNE:
	 		fprintf(f, "bne\t");
        	break;

		case C_BGT:
	 		fprintf(f, "bgt\t");
        	break;

        case C_BLT:
	 		fprintf(f, "blt\t");
        	break;
      
        case C_BGE:
	 		fprintf(f, "bge\t");
        	break;
      
        case C_BLE:
	 		fprintf(f, "ble\t");
        	break;
		
		case C_SEQ:
	 		fprintf(f, "seq\t");
        	break;

        case C_SNE:
	 		fprintf(f, "sne\t");
        	break;

		case C_SGT:
	 		fprintf(f, "sgt\t");
        	break;

        case C_SLT:
	 		fprintf(f, "slt\t");
        	break;
      
        case C_SGE:
	 		fprintf(f, "sge\t");
        	break;
      
        case C_SLE:
	 		fprintf(f, "sle\t");
        	break;

        case C_J:
        	fprintf(f, "j\t");
        	break;

        case C_LABEL:
        	label = 1;
        	break;

        case C_SYSCALL:
        	fprintf(f, "syscall\n");
        	return;
        	break;

         case C_SPACE:
        	break;

	    default:
		    fprintf(f, "*undefined_2*");
    }

    if(inst->addr1 != NULL)
    {
    	if(label == 1)
    		fprintf(f, "\n");

		printAddress(inst->addr1);	
		
		if(label == 1)
    		fprintf(f, ":");
    }

    if(inst->addr2 != NULL)
    {
		fprintf(f, ",\t");
		printAddress(inst->addr2);
	}
	if(inst->addr3 != NULL)
	{
		fprintf(f, ",\t");
		printAddress(inst->addr3);
		fprintf(f, "\n");
	}
	else // Só dois campos em mips
	{
		fprintf(f, "\n");
	}
}

void printInstrList(InstrList list)
{
	while(list != NULL)
    {
  		fprintf(f, "\t");
      	printInstr(list->inst);
      	list = list->next;
    }
}

void printPair(Pair p)
{
	printAddress(p->name);
	printInstrList(p->list);
}

void printStack(Stack st)
{
	fprintf(f, ".data\n");

	while(st != NULL)
    {
   		fprintf(f, "\t%s:\t.space 4\n", st->var); // ou .word 0
      	st = st->next;
    }
    fprintf(f, ".text\n");
}

void printExit()
{
    fprintf(f, "\tli\t$v0,\t10\n\tsyscall\n");
}