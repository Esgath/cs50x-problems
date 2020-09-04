SELECT 
	name
FROM
	people
WHERE
	id IN (
		SELECT
			s.person_id
		FROM
			stars AS s
		JOIN
			movies AS m
				ON s.movie_id = m.id
		WHERE
			m.title = "Toy Story"
	);

