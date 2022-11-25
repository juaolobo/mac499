import re

def limodsat_constant_function(clause_num, b):

	clauses = f"Unit 1 :: Clause      :: {clause_num}\n"
	clauses += f"Unit 2 :: Clause      :: {' '.join([str(clause_num) for i in range(b-1)])}\n"
	clauses += f"Unit 3 :: Negation    :: 2\n"
	clauses += f"Unit 4 :: Equivalence :: 1 3\n"
	function = f"f:\n" + clauses

	return function

def limodsat_rational_equivalence(clause_num1, clause_num2, a):

	clause = f"Unit 1 :: Clause      :: {clause_num1}\n"
	clause += f"Unit 2 :: Clause      :: {' '.join([str(clause_num2) for i in range(a)])}\n"
	clause += f"Unit 3 :: Equivalence :: 1 2\n"	
	function = f"f:\n" + clause

	return function

def limodsat_rational_implication(clause_num1, clause_num2, a, reverse=False):

	clause = f"Unit 1 :: Clause      :: {clause_num1}\n"
	clause += f"Unit 2 :: Clause      :: {' '.join([str(clause_num2) for i in range(a)])}\n"
	clause += f"Unit 3 :: Implication :: 1 2\n" if not reverse else f"Unit 3 :: Implication :: 2 1\n"
	function = f"f:\n" + clause

	return function

def limodsat_maximum_robustness(x_prime_clause_num, x_clause_num, p_clause_num):

	clause = f"Unit 1 :: Clause      :: {x_prime_clause_num}\n"
	clause += f"Unit 2 :: Clause      :: {x_clause_num}\n"
	clause += f"Unit 3 :: Clause      :: {p_clause_num}\n"
	clause += f"Unit 4 :: Disjunction :: 2 3\n"
	clause += f"Unit 5 :: Equivalence :: 1 4\n"

	clause += f"Unit 6 :: Implication :: 2 3\n"
	clause += f"Unit 7 :: Negation    :: 6\n"
	clause += f"Unit 8 :: Equivalence :: 1 7\n"

	clause += f"Unit 9 :: Maximum     :: 5 8\n"

	function = f"f:\n" + clause

	return function

def limodsat_consequence(phi_prime_clause_num, half_clause_num):

	clause = f"Unit 1 :: Clause      :: {phi_prime_clause_num}\n"
	clause += f"Unit 2 :: Clause      :: {half_clause_num}\n"
	clause += f"Unit 3 :: Implication :: 1 2\n"
	function = f"C:\n" + clause

	return function

def limodsat_robustness(filename, a, b, alpha, beta, output="function_robust.limodsat"):

	f = open(filename)
	data = f.read()

	units_str = re.findall(r"Unit (\d+)", data)
	units = [int(u) for u in set(units_str)]
	last_unit = max(units)

	units_statement_str = re.findall(r"-= Formula phi =-\n\n((.*\n)+)\n-= MODSAT Set Phi =-", data)[0][0]
	units_statement = units_statement_str.split("\n")[:-1]

	clauses_nums = [int(n) for n in re.findall("(-?\d+)", data)]
	formulas_str = re.findall(r"-= MODSAT Set Phi =-((.*\n)+)\n", data)[0][0]
	formulas = units_statement_str.split("\n")[:-1]

	last_clause_num = max(clauses_nums)

	# create declaration of clauses
	phi_clause_num = last_clause_num+1
	new_clause = f"Unit {last_unit+1} :: Clause      :: {phi_clause_num}\n"
	new_eq = f"Unit {last_unit+2} :: Equivalence :: {last_unit} {last_unit+1}\n"
	new_clauses = new_clause + new_eq
	new_data = units_statement_str + new_clauses + formulas_str

	b_clause_num = last_clause_num+2
	constant_b = limodsat_constant_function(b_clause_num, b)
	beta_clause_num = last_clause_num+3
	constant_beta = limodsat_constant_function(beta_clause_num, beta)
	half_clause_num = last_clause_num+4
	constant_2 = limodsat_constant_function(half_clause_num, 2)


	# create phi prime
	# account constant functions just created for the offset
	phi_clauses_offset = last_clause_num + 4
	lines = new_data.split("\n")
	new_data += constant_b + constant_beta + constant_2
	phi_prime_lines = 'f:\n'
	for line in lines:
		digits = re.findall(r"Unit (\d+) :: (.*) :: (.*)", line)

		if len(digits) > 0:
			groups = digits[0]

			unit_num = groups[0]
			unit_op = groups[1]

			if "Clause" in unit_op:
				clause_nums = [str(int(d) + phi_clauses_offset) if int(d) >= 0 else str(int(d) - phi_clauses_offset) for d in groups[2].split()]

			else:
				clause_nums = groups[2].split()

			phi_prime_lines += f"Unit {unit_num} :: {unit_op} :: {' '.join(clause_nums)}\n"

		elif 'Formula' in line or 'f:' in line:
			phi_prime_lines += 'f:\n'

	phi_prime_clauses_nums = [int(n) for n in re.findall("(-?\d+)", phi_prime_lines)]
	print(phi_prime_lines)

	max_clause_num = max(phi_prime_clauses_nums)
	# the output variable is the number of the max variable - 3 beacuse of the constant functions we just created
	phi_prime_res_num = max_clause_num

	# create p_n -> alpha*Z_{1/beta}	
	fs = ''
	# for i in range(phi_clause_num): should create a x_prime for the output variable?
	for i in range(last_clause_num):
		fs += limodsat_rational_implication(max_clause_num + i + 1, beta_clause_num, alpha)

	# create maximum functions
	max_fs = ''
	# for i in range(phi_clause_num): should create a x_prime for the output variable?
	for i in range(last_clause_num):
		max_fs += limodsat_maximum_robustness(phi_clauses_offset + i + 1, i + 1, max_clause_num + i + 1)

	# create aZ_{1/b} -> phi
	fs += limodsat_rational_implication(last_clause_num, b_clause_num, a, reverse=True)

	# create Z_{1/2} -> phi_prime
	cons = limodsat_consequence(half_clause_num, phi_prime_res_num)

	new_data = re.sub(r"Formula \d+:", "f:", new_data)
	new_data = re.sub(r"-=(.*)=-\n*", "", new_data)
	new_data = "Cons\n" + "f:\n" + new_data + phi_prime_lines + fs + max_fs + cons
	# print(phi_prime_lines)
	# print(phi_clauses_offset)

	with open(output, "w") as f:
		f.write(new_data)


def limodsat_reachability(filename, a, b, equivalence=True, output="function_reach.limodsat"):
	
	f = open(filename)
	data = f.read()

	units_str = re.findall(r"Unit (\d+)", data)
	units = [int(u) for u in set(units_str)]
	last_unit = max(units)

	units_statement_str = re.findall(r"-= Formula phi =-\n\n((.*\n)+)\n-= MODSAT Set Phi =-", data)[0][0]
	units_statement = units_statement_str.split("\n")[:-1]

	clauses_nums = [int(n) for n in re.findall("(-?\d+)", data)]
	formulas_str = re.findall(r"-= MODSAT Set Phi =-((.*\n)+)\n", data)[0][0]
	formulas = units_statement_str.split("\n")[:-1]

	new_clause_num = max(clauses_nums)

	# create declaration of clauses
	new_clause = f"Unit {last_unit+1} :: Clause      :: {new_clause_num+1}\n"
	new_eq = f"Unit {last_unit+2} :: Equivalence :: {last_unit} {last_unit+1}\n"
	new_clauses = new_clause + new_eq

	new_function_1 = limodsat_constant_function(new_clause_num+2, b)

	if equivalence:
		new_function_2 = limodsat_rational_equivalence(new_clause_num+1, new_clause_num+2, a)

	else:
		new_function_2 = limodsat_rational_implication(new_clause_num+1, new_clause_num+2, a)

	new_data = units_statement_str + new_clauses + formulas_str + new_function_1 + new_function_2
	new_data = re.sub(r"Formula \d+:", "f:", new_data)
	new_data = re.sub(r"-=(.*)=-\n*", "", new_data)
	new_data = "Sat\n" + "f:\n" + new_data
	# print(new_data)

	with open(output, "w") as f:
		f.write(new_data)


if __name__ == "__main__":

	N = 10
	output_dir = 'tests/'
	for i in range(1, N):
		limodsat_reachability("function.limodsat", i, N, equivalence=False, output=(output_dir + f"reachability_{i}-{N}.limodsat"))

	pi = [3, 4]
	limits = [1, 10, 20, 25, 40, 50]
	N = 100
	for l in limits:
		limodsat_robustness("function.limodsat", *pi, l, N, output=(output_dir + f"robustness_{pi[0]}-{pi[1]}_{l}-{N}.limodsat"))