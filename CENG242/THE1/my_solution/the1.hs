--{-# OPTIONS_GHC -Wall #-}
module Hw1 where

    type Mapping = [(String, String, String)]
    data AST = EmptyAST | ASTNode String AST AST deriving (Show, Read)
    
    writeExpression :: (AST, Mapping) -> String
    evaluateAST :: (AST, Mapping) -> (AST, String)
    -- DO NOT MODIFY OR DELETE THE LINES ABOVE -- 
    -- IMPLEMENT writeExpression and evaluateAST FUNCTION ACCORDING TO GIVEN SIGNATURES -- 
    
    --writeExpression implementation--
    
    writeExpression (EmptyAST, []) = []
    writeExpression (ASTNode root left right, []) 
            | root == "plus" = "(" ++ writeExpression (left,[]) ++ "+" ++ writeExpression (right,[]) ++ ")"
            | root == "times" = "(" ++ writeExpression (left,[]) ++ "*" ++ writeExpression (right,[]) ++ ")"
            | root == "negate" = "(" ++ "-" ++ writeExpression (left,[]) ++ ")"
            | root == "cat" = "(" ++ writeExpression(left,[]) ++ "++" ++  writeExpression(right,[]) ++ ")"
            | root == "len" = "(" ++ "length " ++ writeExpression(left,[]) ++ ")"
            | root == "num" = let (ASTNode value _ _ ) = left in value
            | root == "str" = let (ASTNode value _ _ ) = left in "\"" ++ value ++ "\""
            | otherwise = root
    writeExpression ((ASTNode root left right), (var, valType, value):xs) =
            "let " ++ variablePrinter((var, valType, value):xs) ++ " in " ++ writeExpression ((ASTNode root left right),[])
    
    variablePrinter :: (Mapping) -> String
    variablePrinter ((var, valType, value):[]) 
        | valType == "num" =  var ++ "=" ++ value
        | valType == "str" =  var ++ "=" ++ "\"" ++ value  ++ "\""
    variablePrinter ((var, valType, value):xs)
        | valType == "num" = var ++ "=" ++ value ++ ";" ++ variablePrinter(xs) 
        | valType == "str" = var ++ "=" ++ "\"" ++ value  ++ "\"" ++ ";" ++ variablePrinter(xs) 
    
    --evaluateAST implementation--
    
    insertVariable :: (AST,(String,String,String)) -> AST
    insertVariable(EmptyAST,_) = EmptyAST
    insertVariable(ASTNode root left right, (var, valType, value)) 
        | root == var = ASTNode valType (ASTNode value EmptyAST EmptyAST) EmptyAST
        | otherwise   = ASTNode root (insertVariable(left, (var, valType, value))) (insertVariable(right, (var, valType, value)))
    
    insertAllVariables :: (AST, Mapping) -> AST
    insertAllVariables(ASTNode root left right, []) = ASTNode root left right
    insertAllVariables(ASTNode root left right, all@(x:xs)) = 
        insertAllVariables(insertVariable(ASTNode root left right,x),xs)
    
    data Expr = Num String | Str String | Add Expr Expr | Times Expr Expr | Cat Expr Expr | Negate Expr | Len Expr deriving (Show, Read)
    
    readExpr :: (AST) -> Expr
    readExpr(ASTNode root left right) 
        | root == "plus" = Add (readExpr(left)) (readExpr(right))
        | root == "times" = Times (readExpr(left)) (readExpr(right))
        | root == "negate" = Negate (readExpr(left))
        | root == "cat" = Cat (readExpr(left)) (readExpr(right))
        | root == "len" = Len (readExpr(left))
        | root == "num" = let (ASTNode value _ _ ) = left in Num value
        | root == "str" = let (ASTNode value _ _ ) = left in Str value
    
    exprEval :: (Expr) -> String
    exprEval(Num n) = n 
    exprEval(Str s) = s 
    exprEval(Add x y) = show ((read (exprEval(x))::Int) + (read (exprEval(y))::Int))
    exprEval(Times x y) = show ((read (exprEval(x))::Int) * (read (exprEval(y))::Int))
    exprEval(Cat x y) = exprEval(x) ++ exprEval(y)
    exprEval(Negate x) = show ((read (exprEval(x))::Int) * (-1))
    exprEval(Len x) = show (length(exprEval(x)))
    
    evaluateAST(ASTNode root left right,variablesList) = (x, exprEval(readExpr(x)))
                                                       where x = insertAllVariables(ASTNode root left right, variablesList)