all:
	g++ main.cpp -o m
	./m < a_example.in > a.out
	./m < b_should_be_easy.in > b.out
	./m < c_no_hurry.in > c.out
	./m < d_metropolis.in > d.out
	./m < e_high_bonus.in > e.out
