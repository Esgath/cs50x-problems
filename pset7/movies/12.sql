SELECT
	m.title
FROM
	movies AS m
JOIN
	stars AS s
		ON s.movie_id = m.id
JOIN
	people AS p
		ON p.id = s.person_id
WHERE
	m.id IN (
		SELECT
			m.id
		FROM
			movies AS m
		JOIN
			stars AS s
				ON s.movie_id = m.id
		JOIN 
			people AS p
				ON p.id = s.person_id
		WHERE
			p.name = "Johnny Depp"
	) AND p.name = "Helena Bonham Carter";