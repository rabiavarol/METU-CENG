module Hw2 where

data ASTResult = ASTError String | ASTJust (String, String, Int) deriving (Show, Read)
data ASTDatum = ASTSimpleDatum String | ASTLetDatum String deriving (Show, Read)
data AST = EmptyAST | ASTNode ASTDatum AST AST deriving (Show, Read)

isNumber :: String -> Bool
eagerEvaluation :: AST -> ASTResult
normalEvaluation :: AST -> ASTResult
-- DO NOT MODIFY OR DELETE THE LINES ABOVE -- 
-- IMPLEMENT isNumber, eagerEvaluation and normalEvaluation FUNCTIONS ACCORDING TO GIVEN SIGNATURES -- 

isDigit x
    | x `elem` ['0'..'9'] = True
    | otherwise = False

isNumber' [] = True
isNumber'(x:xs) = isDigit x && isNumber'(xs)

isNumber [] = False
isNumber (x:xs)
    | x `elem` ['0'..'9'] = isNumber'(xs)
    | x == '-' && length xs /= 0 = isNumber'(xs)
    | otherwise = False
    
data Expr = Var String | Num String | Str String | Add Expr Expr | Times Expr Expr | Cat Expr Expr | Negate Expr | Len Expr | Let String Expr Expr deriving (Show, Read)

readExpr :: (AST) -> Expr
readExpr(ASTNode (ASTLetDatum root) left right)  = Let root (readExpr(left)) (readExpr(right))
readExpr(ASTNode (ASTSimpleDatum root) left right) 
    | root == "plus" = Add (readExpr(left)) (readExpr(right))
    | root == "times" = Times (readExpr(left)) (readExpr(right))
    | root == "negate" = Negate (readExpr(left))
    | root == "cat" = Cat (readExpr(left)) (readExpr(right))
    | root == "len" = Len (readExpr(left))
    | root == "num" = let (ASTNode (ASTSimpleDatum value) _ _ ) = left in Num value
    | root == "str" = let (ASTNode (ASTSimpleDatum value) _ _ ) = left in Str value
    | otherwise = Var root

type Env = [(String, Expr)]

normalTree:: Env -> Expr -> Expr

normalTree env (Var x) = n where Just n = lookup x env 
normalTree _ (Num x) = Num x
normalTree _ (Str x) = Str x
normalTree env (Add x y) = Add (normalTree env x) (normalTree env y) 
normalTree env (Times x y) = Times (normalTree env x) (normalTree env y) 
normalTree env (Cat x y) = Cat (normalTree env x) (normalTree env y) 
normalTree env (Negate x) = Negate (normalTree env x) 
normalTree env (Len x) = Len (normalTree env x) 
normalTree env (Let var left right) = normalTree ((var,(normalTree env left)) : env) right
   
evalTree:: Expr -> String
evalTree(Num n) = n 
evalTree(Str s) = s 
evalTree(Add x y) = show ((read (evalTree(x))::Int) + (read (evalTree(y))::Int))
evalTree(Times x y) = show ((read (evalTree(x))::Int) * (read (evalTree(y))::Int))
evalTree(Cat x y) = evalTree(x) ++ evalTree(y)
evalTree(Negate x) = show ((read (evalTree(x))::Int) * (-1))
evalTree(Len x) = show (length(evalTree(x)))

countSteps:: Expr -> Int
countSteps (Var _) = 0
countSteps (Num _) = 0
countSteps (Str _) = 0
countSteps (Add x y) = 1 + (countSteps x) + (countSteps y)
countSteps (Times x y) = 1 + (countSteps x) + (countSteps y)
countSteps (Cat x y) = 1 + (countSteps x) + (countSteps y) 
countSteps (Negate x) = 1 + (countSteps x) 
countSteps (Len x) = 1 + (countSteps x) 
countSteps (Let _ left right) = (countSteps left) + (countSteps right)

typeOfResult:: String -> String
typeOfResult x 
    | isNumber x = "num"
    | otherwise = "str"

errorHandling:: Expr -> String
errorHandling (Num n) 
    | isNumber n = "num"
    | otherwise = "the value " ++ "'" ++ n ++ "'" ++ " is not a number!"
errorHandling (Str n) = "str"
errorHandling (Add left right) 
    | errorHandling(left) == "num" && errorHandling(right) == "str" = "plus operation is not defined between num and str!"
    | errorHandling(left) == "str" && errorHandling(right) == "num" = "plus operation is not defined between str and num!"
    | errorHandling(left) == "str" && errorHandling(right) == "str" = "plus operation is not defined between str and str!"
    | errorHandling(left) !! 1 == 'a' = errorHandling(left)
    | errorHandling(right) !! 1 == 'a' = errorHandling(right)
    | errorHandling(left) !! 1 == 'h' = errorHandling(left)
    | errorHandling(right) !! 1 == 'h' = errorHandling(right)
    | otherwise = "num"
errorHandling (Times left right)   
    | errorHandling(left) == "num" && errorHandling(right) == "str" = "times operation is not defined between num and str!"
    | errorHandling(left) == "str" && errorHandling(right) == "num" = "times operation is not defined between str and num!"
    | errorHandling(left) == "str" && errorHandling(right) == "str" = "times operation is not defined between str and str!"
    | errorHandling(left) !! 1 == 'h' = errorHandling(left)
    | errorHandling(right) !! 1 == 'h' = errorHandling(right)
    | otherwise = "num"
errorHandling (Cat left right)   
    | errorHandling(left) == "num" && errorHandling(right) == "str" = "cat operation is not defined between num and str!"
    | errorHandling(left) == "str" && errorHandling(right) == "num" = "cat operation is not defined between str and num!"
    | errorHandling(left) == "num" && errorHandling(right) == "num" = "cat operation is not defined between num and num!"
    | errorHandling(left) !! 1 == 'h' = errorHandling(left)
    | errorHandling(right) !! 1 == 'h' = errorHandling(right)
    | otherwise = "str"
errorHandling (Len left)
    | errorHandling(left) == "num" = "len operation is not defined on num!"
    | errorHandling(left) !! 1 == 'h' = errorHandling(left)
    | otherwise = "num"
errorHandling (Negate left)
    | errorHandling(left) == "str" = "negate operation is not defined on str!"
    | errorHandling(left) !! 1 == 'h' = errorHandling(left)
    | otherwise = "num"

normalEvaluation(ASTNode root left right) 
    | errorHandling(normalTree [] (readExpr(ASTNode root left right))) == "num" = ASTJust(res,resType,step)
    | errorHandling(normalTree [] (readExpr(ASTNode root left right))) == "str" = ASTJust(res,resType,step)
    | otherwise = ASTError (errorHandling(normalTree [] (readExpr(ASTNode root left right))))
    where 
        res = evalTree(normalTree [] (readExpr(ASTNode root left right)))
        resType = typeOfResult res
        step = countSteps(normalTree [] (readExpr(ASTNode root left right)))

eagerEvaluation(ASTNode root left right) 
    | errorHandling(normalTree [] (readExpr(ASTNode root left right))) == "num" = ASTJust(res,resType,step)
    | errorHandling(normalTree [] (readExpr(ASTNode root left right))) == "str" = ASTJust(res,resType,step)
    | otherwise = ASTError (errorHandling(normalTree [] (readExpr(ASTNode root left right))))
    where 
        res = evalTree(normalTree [] (readExpr(ASTNode root left right)))
        resType = typeOfResult res
        step = countSteps((readExpr(ASTNode root left right)))






