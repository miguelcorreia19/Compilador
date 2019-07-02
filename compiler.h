int countVar = 0;
int countLabel = 1;
Stack stack = NULL;

char* newVar()
{
  int temp;
  char c = 't';
  if(countVar > 9)
  {
    temp = countVar - 9;
    c = 's';
  }
  else if(countVar > 16)
  {
    countVar = 0;
    temp = countVar;
  }
  else
    temp = countVar;

  char *var = malloc(sizeof(int) + 2);     
  sprintf(var, "$%c%d", c, temp);
  
  countVar++;  
  return var;
}

char* newLabel()
{
  char *label = malloc(sizeof(int) + 5);     
  sprintf(label, "label%d", countLabel);
  
  countLabel++;  

  return label;
}

Pair Expressions(Expr expr) 
{
  if (expr->kind == E_INTEGER) 
  {
    return C_mkPair(C_mkInt(expr->attr.value), NULL);  
  } 
  
  else if (expr->kind == E_VARIABLE) 
  {
    if(C_mkStackContains(expr->attr.var, stack) == 0)
      stack = C_mkStack(expr->attr.var, stack);
    
    char *var = newVar();

    Instr inst = C_mkInstr(C_LW, C_mkChar(var), C_mkChar(expr->attr.var), NULL);

    return C_mkPair(C_mkChar(var), C_mkInstrList(inst, NULL));
  }

  else if (expr->kind == E_OPERATION) 
  {
    Pair left = Expressions(expr->attr.op.left);
    Pair right = Expressions(expr->attr.op.right);

    // add $v, int, $v
    // slt $v, $v, int
    // casos erro
    if(left->name->kind == C_INT)
    {
      Instr inst = C_mkInstr(C_LI, C_mkChar(newVar()), left->name, NULL);

      left->name = inst->addr1;
      left->list = C_mkInstrList(inst, NULL);
    }

    if(right->name->kind == C_INT)
    {
      Instr inst = C_mkInstr(C_LI, C_mkChar(newVar()), right->name, NULL);

      right->name = inst->addr1;
      right->list = C_mkInstrList(inst, NULL);
    }
    char *var = newVar();

    OpKind op;

    switch(expr->attr.op.operator) 
    {
      case PLUS: 
          op = C_ADD;
        break;

      case MINUS: 
          op = C_SUB;
        break;

      case DIV: 
        op = C_DIV;
        break;
        
      case MULT: 
        op = C_MUL;
        break;

      case EQUAL: 
        op = C_SEQ;
        break;

      case NEQUAL: 
        op = C_SNE;
        break;

      case MORE: 
        op = C_SGT;
        break;

      case LESS: 
        op = C_SLT;
        break;

      case MOREeq: 
        op = C_SGE;
        break;

      case LESSeq: 
        op = C_SLE;
        break;
    }

    Instr inst = C_mkInstr(op, C_mkChar(var), left->name, right->name);
      
    InstrList list = append(left->list, right->list);

    // Instr -> InstrList só com um argumento -> append das duas listas
    list = append(list, C_mkInstrList(inst, NULL));

    return C_mkPair(C_mkChar(var), list);
  }

  else if (expr->kind == E_OPBOOLEAN)
  {
    
    Pair left = Expressions(expr->attr.op.left);
    Pair right = Expressions(expr->attr.op.right);

    if(left->name->kind == C_INT)
    {
      Instr inst = C_mkInstr(C_LI, C_mkChar(newVar()), left->name, NULL);

      left->name = inst->addr1;
      left->list = C_mkInstrList(inst, NULL);
    }

    if(right->name->kind == C_INT)
    {
      Instr inst = C_mkInstr(C_LI, C_mkChar(newVar()), right->name, NULL);

      right->name = inst->addr1;
      right->list = C_mkInstrList(inst, NULL);
    }

    char *label = newLabel();

    OpKind op;

    switch(expr->attr.op.operator) 
    {
      case EQUAL: 
        op = C_BNE;
        break;

      case NEQUAL: 
        op = C_BEQ;
        break;

      case MORE: 
        op = C_BLE;
        break;

      case LESS: 
        op = C_BGE;
        break;

      case MOREeq: 
        op = C_BLT;
        break;

      case LESSeq: 
        op = C_BGT;
        break;
    }

    Instr inst = C_mkInstr(op, left->name, right->name, C_mkChar(label));
    InstrList list = append(left->list, right->list);
    
    // Instr -> InstrList só com um argumento -> append das duas listas
    list = append(list, C_mkInstrList(inst, NULL));

    return C_mkPair(NULL, list);
  }

  else if (expr->kind == E_BRACKETS) 
  {
    return Expressions(expr->attr.exp);
  }

  else if (expr->kind == E_INPUT) 
  {
    Instr inst = C_mkInstr(C_LI, C_mkChar("$v0"), C_mkInt(5), NULL);
    InstrList list = NULL;
    list = append(list, C_mkInstrList(inst, NULL));

    inst = C_mkInstr(C_SYSCALL, NULL, NULL, NULL);
    list = append(list, C_mkInstrList(inst, NULL));

    inst = C_mkInstr(C_MOVE, C_mkChar(newVar()), C_mkChar("$v0"), NULL);
    list = append(list, C_mkInstrList(inst, NULL));

    return C_mkPair(inst->addr1, list);
  }
}

InstrList Commands(cmd command) 
{
  countVar = 0;
  if (command->kind == E_ATRIB) 
  {
    if(C_mkStackContains(command->attr.atrib.var, stack) == 0)
      stack = C_mkStack(command->attr.atrib.var, stack);

    Pair exp = Expressions(command->attr.atrib.exp);

    // nao fazer sw de int
    if(exp->name->kind == C_INT)
    {
      Instr inst = C_mkInstr(C_LI, C_mkChar(newVar()), exp->name, NULL);
      
      exp->name = inst->addr1;
      exp->list = C_mkInstrList(inst, NULL);
    }

    Instr inst = C_mkInstr(C_SW, exp->name, C_mkChar(command->attr.atrib.var), NULL);

    // lado direito do para ++ nova instr que passou a instrList
    return append(exp->list, C_mkInstrList(inst, NULL));
  } 

  else if(command->kind == E_IF)
  {
    // IF
    char *exitLabel = newLabel();

    // boolean condition
    Pair cond = Expressions(command->attr.iff.cond); 

    InstrList list = NULL;
    Address label = last(cond->list)->addr3;

    if(command->attr.iff.elseiff == NULL && command->attr.iff.elsee == NULL)
      last(cond->list)->addr3 = C_mkChar(exitLabel);

    list = append(list, cond->list);

    // body
    cmdList cmd = command->attr.iff.body; 
    while(cmd!=NULL)
    { 
      list = append(list, Commands(cmd->head));
      cmd = cmd->tail;
    }

    Instr inst;
    if(command->attr.iff.elseiff != NULL || command->attr.iff.elsee != NULL)
    {
      inst = C_mkInstr(C_J, C_mkChar(exitLabel), NULL, NULL);
      list = append(list, C_mkInstrList(inst, NULL));
    }

    // ELSEIF
    
    elseifList listElseIf = command->attr.iff.elseiff; 

    while(listElseIf != NULL)
    { 
      inst = C_mkInstr(C_LABEL, label, NULL, NULL);
      list = append(list, C_mkInstrList(inst, NULL));

      // one elseif
      elseif cmdElseIf = listElseIf->head;
     
      // condition (bool)
      cond = Expressions(cmdElseIf->cond);
      list = append(list, cond->list);
      
      label = last(cond->list)->addr3;

      if(listElseIf->tail == NULL && command->attr.iff.elsee == NULL)
        last(cond->list)->addr3 = C_mkChar(exitLabel);

      // body
      cmd = cmdElseIf->body; 
      while(cmd != NULL)
      { 
        list = append(list, Commands(cmd->head));
        cmd = cmd->tail;
      }

      if(listElseIf->tail != NULL || command->attr.iff.elsee != NULL)
      {
        inst = C_mkInstr(C_J, C_mkChar(exitLabel), NULL, NULL);
        list = append(list, C_mkInstrList(inst, NULL));
      }

      listElseIf = listElseIf->tail;
    }

    // ELSE
    if(command->attr.iff.elsee != NULL)
    {
      inst = C_mkInstr(C_LABEL, label, NULL, NULL);
      list = append(list, C_mkInstrList(inst, NULL));

      // body
      cmd = command->attr.iff.elsee;
      while(cmd != NULL)
      { 
        list = append(list, Commands(cmd->head));
        cmd = cmd->tail;
      }
    }

    inst = C_mkInstr(C_LABEL, C_mkChar(exitLabel), NULL, NULL);
    list = append(list, C_mkInstrList(inst, NULL));
    return list;
  }

  else if(command->kind == E_WHILE)
  {
    char *startLabel = newLabel();
    
    InstrList list = NULL;
    Instr inst = C_mkInstr(C_LABEL, C_mkChar(startLabel), NULL, NULL);
    list = append(list, C_mkInstrList(inst, NULL));

    // boolean condition (if)
    Pair cond = Expressions(command->attr.whilee.cond); 

    Address exitLabel = last(cond->list)->addr3;
    list = append(list, cond->list);

    // body
    cmdList cmd = command->attr.whilee.body; 
    while(cmd != NULL)
    { 
      list = append(list, Commands(cmd->head));
      cmd = cmd->tail;
    }

    inst = C_mkInstr(C_J, C_mkChar(startLabel), NULL, NULL);
    list = append(list, C_mkInstrList(inst, NULL));

    inst = C_mkInstr(C_LABEL, exitLabel, NULL, NULL);
    list = append(list, C_mkInstrList(inst, NULL));
    
    return list;
  }

  else if(command->kind == E_FOR)
  {
    // add to list left side
    InstrList list = Commands(command->attr.forr.decl);

    char *startLabel = newLabel();

    // variavel de comparação
    Address startVar = last(list)->addr2;

    // new label
    Instr inst = C_mkInstr(C_LABEL, C_mkChar(startLabel), NULL, NULL);
    list = append(list, C_mkInstrList(inst, NULL));

    // criar o if
    Expr ifcondition = ast_boolean(LESSeq, ast_var(startVar->content.var), command->attr.forr.cond);
    cmd ifcommand = mkIf(ifcondition, NULL, NULL, NULL);

    list = append(list, Commands(ifcommand));
  
    // apagar a ultima linha do if (sem else) (jump)
    list = removeLast(list);

    // buscar o label final (if)
    Address exitLabel = last(list)->addr3;

    // body
    cmdList cmmd = command->attr.forr.body; 
    
    while(cmmd!=NULL)
    { 
      list = append(list, Commands(cmmd->head));
      cmmd = cmmd->tail;
    }

    // incrementar var
    Expr increment = ast_operation(PLUS, ast_var(startVar->content.var), ast_integer(1));
    cmd incrementcmd = mkAtrib(startVar->content.var, increment);
    list = append(list, Commands(incrementcmd));

    // salta para o inicio
    inst = C_mkInstr(C_J, C_mkChar(startLabel), NULL, NULL);
    list = append(list, C_mkInstrList(inst, NULL));

    inst = C_mkInstr(C_LABEL, exitLabel, NULL, NULL);
    list = append(list, C_mkInstrList(inst, NULL));
  }

  else if(command->kind == E_OUTPUT)
  {
    InstrList list = NULL;
    Pair p = Expressions(command->attr.output);

    list = append(list, p->list);
    Instr inst = C_mkInstr(C_LI, C_mkChar("$v0"), C_mkInt(1), NULL);
    list = append(list, C_mkInstrList(inst, NULL));

    if(p->name->kind == C_INT)
    {
      Instr inst = C_mkInstr(C_LI, C_mkChar(newVar()), p->name, NULL);
      list = append(list, C_mkInstrList(inst, NULL));
      p->name = inst->addr1;
    }

    inst = C_mkInstr(C_MOVE, C_mkChar("$a0"), p->name, NULL);
    list = append(list, C_mkInstrList(inst, NULL));

    inst = C_mkInstr(C_SYSCALL, NULL, NULL, NULL);
    list = append(list, C_mkInstrList(inst, NULL));

    
    return list;
  }  
}
